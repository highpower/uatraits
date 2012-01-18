#include "acsetup.hpp"

#include <boost/python.hpp>
#include "uatraits/details/detector_impl.hpp"

namespace uatraits { namespace python {

namespace py = boost::python;

class python_detector {

public:
	python_detector(char const *file);
	py::dict detect(py::str str) const;
	py::dict checked_detect(py::str str, py::object &obj) const;

private:
	typedef details::detector_impl<py::dict> impl_type;
	shared_ptr<impl_type> impl_;
};

python_detector::python_detector(char const *file)
{
	using namespace details;
	resource<xmlDocPtr, xml_doc_traits> doc(xmlParseFile(file));
	xml_throw_unless(doc);
	impl_.reset(new impl_type(doc.get()));
}

py::dict
python_detector::detect(py::str str) const {
	py::dict result;
	char const *data = py::extract<char const*>(str);
	std::string::size_type size = static_cast<std::string::size_type>(py::len(str));
	impl_->detect(data, data + size, result);
	return result;
}

py::dict
python_detector::checked_detect(py::str str, py::object &obj) const {
	py::dict result;
	char const *data = py::extract<char const*>(str);
	std::string::size_type size = static_cast<std::string::size_type>(py::len(str));
	impl_->detect(data, data + size, result);
	return result;
}

BOOST_PYTHON_MODULE(uatraits) {
	py::class_<python_detector> cl("detector", py::init<char const*>());
	cl.def("detect", &python_detector::detect);
	cl.def("checked_detect", &python_detector::detect);
}

}} // namespaces
