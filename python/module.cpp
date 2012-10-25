#include "acsetup.hpp"

#include <boost/python.hpp>

#include "streambuf.hpp"
#include "dict_wrapper.hpp"
#include "uatraits/details/detector_impl.hpp"

namespace uatraits {

namespace details {

template<> inline std::string
detector_impl<python::dict_wrapper>::find_header(const python::dict_wrapper &headers, const std::string &header) const {
	return headers.get(header);
}

template<> inline python::dict_wrapper
detector_impl<python::dict_wrapper>::detect_by_header(const std::string &header) const {
	py::dict copy;

	python::dict_wrapper dict(copy);

	detect(header.c_str(), header.c_str() + header.size(), dict);

	return dict;
}

template <> inline void
detector_impl<python::dict_wrapper>::update_attr(python::dict_wrapper &traits, python::dict_wrapper &copy,
	const std::string &header) const {
	if (!copy.has(header)) {
		traits[header] = copy.get(header);
	}
}

} // namespace details

namespace python {

namespace py = boost::python;

class python_detector {

public:
	python_detector(char const *file, char const *profiles = NULL);
	py::dict detect(py::str str) const;
	py::dict detect_by_headers(py::dict dict) const;
	py::dict checked_detect(py::str str, py::object &obj) const;

private:
	typedef details::detector_impl<dict_wrapper> impl_type;
	shared_ptr<impl_type> impl_;
};

python_detector::python_detector(char const *file, char const *profiles)
{
	using namespace details;
	resource<xmlDocPtr, xml_doc_traits> doc(xmlReadFile(file, 0, XML_PARSE_NOENT));
	xml_throw_unless(doc);
	if (NULL == profiles) {
		impl_.reset(new impl_type(doc.get()));
	}
	else {
		resource<xmlDocPtr, xml_doc_traits> profiles_doc(xmlReadFile(profiles, 0, XML_PARSE_NOENT));
		xml_throw_unless(profiles_doc);
		impl_.reset(new impl_type(doc.get(), profiles_doc.get()));
	}
}

py::dict
python_detector::detect(py::str str) const {
	py::dict result;
	dict_wrapper wrapper(result);
	char const *data = py::extract<char const*>(str);
	std::string::size_type size = static_cast<std::string::size_type>(py::len(str));
	impl_->detect(data, data + size, wrapper);
	return result;
}

py::dict
python_detector::detect_by_headers(py::dict headers) const {
	py::dict result;
	dict_wrapper wrapper(result);
	dict_wrapper headers_wrapper(headers);

	impl_->detect(headers_wrapper, wrapper);
	return result;
}

py::dict
python_detector::checked_detect(py::str str, py::object &obj) const {
	(void)obj;
	py::dict result;
	dict_wrapper wrapper(result);
	char const *data = py::extract<char const*>(str);
	std::string::size_type size = static_cast<std::string::size_type>(py::len(str));
	impl_->detect(data, data + size, wrapper);
	return result;
}

BOOST_PYTHON_MODULE(uatraits) {
	py::class_<python_detector> cl("detector", py::init<char const*, py::optional<char const*> >());
	cl.def("detect", &python_detector::detect);
	cl.def("detect_by_headers", &python_detector::detect_by_headers);
	cl.def("checked_detect", &python_detector::detect);
}

}} // namespaces
