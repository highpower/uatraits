#include <map>
#include <string>

#include <node.h>
#include <v8.h>

#include "uatraits/details/detector_impl.hpp"

#include "dict_wrapper.hpp"

namespace uatraits {

namespace details {

template<> inline std::string
detector_impl<nodejs::dict_wrapper>::find_header(const nodejs::dict_wrapper &headers, const std::string &header) const {
	return headers.get(header);
}

template<> inline nodejs::dict_wrapper
detector_impl<nodejs::dict_wrapper>::detect_by_header(const std::string &header) const {
	v8::Local<v8::Object> copy = v8::Object::New();

	nodejs::dict_wrapper dict(copy);

	detect(header.c_str(), header.c_str() + header.size(), dict);

	return dict;
}

template <> inline void
detector_impl<nodejs::dict_wrapper>::update_attr(nodejs::dict_wrapper &traits, nodejs::dict_wrapper &copy,
	const std::string &header) const {
	if (!copy.has(header)) {
		traits[header] = copy.get(header);
	}
}

} // namespace details

namespace nodejs {

class Detector : public node::ObjectWrap {

private:
	static v8::Persistent<v8::FunctionTemplate> ptemplate;

public:
	Detector() {
	}
	virtual ~Detector() {
	}

public:
	static void Init(v8::Handle<v8::Object> target) {
		v8::HandleScope scope;

		v8::Local<v8::FunctionTemplate> ltemplate = v8::FunctionTemplate::New(New);

		ptemplate = v8::Persistent<v8::FunctionTemplate>::New(ltemplate);
		ptemplate->InstanceTemplate()->SetInternalFieldCount(1);
		ptemplate->SetClassName(v8::String::NewSymbol("Detector"));

		NODE_SET_PROTOTYPE_METHOD(ptemplate, "detect", Detect);
		NODE_SET_PROTOTYPE_METHOD(ptemplate, "detect_by_headers", DetectByHeaders);

		target->Set(v8::String::NewSymbol("Detector"), ptemplate->GetFunction());
	}

	static v8::Handle<v8::Value> New(const v8::Arguments &args) {
		v8::HandleScope scope;

		using namespace details;

		if (args.Length() == 1 && args.Length() == 2) {
			return v8::ThrowException(v8::Exception::TypeError(v8::String::New("too many arguments")));
		}

		if (!args[0]->IsString()) {
			return v8::ThrowException(v8::Exception::TypeError(v8::String::New("first argument must be a string")));
		}

		v8::String::Utf8Value file(v8::Local<v8::String>::Cast(args[0]));

		resource<xmlDocPtr, xml_doc_traits> doc(xmlReadFile(*file, 0, XML_PARSE_NOENT));
		xml_throw_unless(doc);

		Detector *detector = new Detector();

		if (args.Length() == 2) {
			if (!args[1]->IsString()) {
				return v8::ThrowException(v8::Exception::TypeError(v8::String::New(
					"second argument must be a string")));
			}

			v8::String::Utf8Value profiles(v8::Local<v8::String>::Cast(args[1]));

			resource<xmlDocPtr, xml_doc_traits> profiles_doc(xmlReadFile(*profiles, 0, XML_PARSE_NOENT));
			xml_throw_unless(profiles_doc);

			detector->impl_.reset(new impl_type(doc.get(), profiles_doc.get()));
		}
		else {
			detector->impl_.reset(new impl_type(doc.get()));
		}

		detector->Wrap(args.This());

		return args.This();
	}

	static v8::Handle<v8::Value> Detect(const v8::Arguments &args) {
		v8::HandleScope scope;

		Detector *detector = node::ObjectWrap::Unwrap<Detector>(args.This());

		if (args.Length() != 1 || !args[0]->IsString()) {
			return v8::ThrowException(v8::Exception::TypeError(v8::String::New("first argument must be a string")));
		}

		v8::Local<v8::Object> result = v8::Object::New();
		dict_wrapper wrapper(result);

		v8::String::Utf8Value data(v8::Local<v8::String>::Cast(args[0]));
		std::string::size_type size = data.length();

		detector->impl_->detect(*data, *data + size, wrapper);

		return result;
	}

	static v8::Handle<v8::Value> DetectByHeaders(const v8::Arguments &args) {
		v8::HandleScope scope;

		Detector *detector = node::ObjectWrap::Unwrap<Detector>(args.This());

		if (args.Length() != 1 || !args[0]->IsObject()) {
			return v8::ThrowException(v8::Exception::TypeError(v8::String::New("first argument must be an object")));
		}

		v8::Local<v8::Object> result = v8::Object::New();
		v8::Local<v8::Object> headers = v8::Local<v8::Object>::Cast(args[0]);
		dict_wrapper wrapper(result);
		dict_wrapper headers_wrapper(headers);

		detector->impl_->detect(headers_wrapper, wrapper);

		return result;
	}

private:
	typedef details::detector_impl<dict_wrapper> impl_type;
	shared_ptr<impl_type> impl_;

};

v8::Persistent<v8::FunctionTemplate> Detector::ptemplate;

} // namespace nodejs

} // namespace uatraits

extern "C" {

void init(v8::Handle<v8::Object> target){
	v8::HandleScope scope;

	uatraits::nodejs::Detector::Init(target);
}

} // extert "C"

