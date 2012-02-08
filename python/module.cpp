#include "acsetup.hpp"

#include <boost/python.hpp>

#include "streambuf.hpp"
#include "dict_wrapper.hpp"
#include "uatraits/details/detector_impl.hpp"

namespace uatraits { namespace python {

namespace py = boost::python;

class python_detector {

public:
	python_detector(char const *file);
	py::dict detect(py::str str) const;
	py::dict checked_detect(py::str str, py::object &obj) const;

private:
	typedef details::detector_impl<dict_wrapper> impl_type;
	shared_ptr<impl_type> impl_;
};

python_detector::python_detector(char const *file)
{
	using namespace details;
	resource<xmlDocPtr, xml_doc_traits> doc(xmlReadFile(file, 0, XML_PARSE_NOENT));
	xml_throw_unless(doc);
	impl_.reset(new impl_type(doc.get()));
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
python_detector::checked_detect(py::str str, py::object &obj) const {
	py::dict result;
	dict_wrapper wrapper(result);
	char const *data = py::extract<char const*>(str);
	std::string::size_type size = static_cast<std::string::size_type>(py::len(str));
	impl_->detect(data, data + size, wrapper);
	return result;
}

BOOST_PYTHON_MODULE(uatraits) {
	py::class_<python_detector> cl("detector", py::init<char const*>());
	cl.def("detect", &python_detector::detect);
	cl.def("checked_detect", &python_detector::detect);
}

}} // namespaces
