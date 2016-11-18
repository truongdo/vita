%module vncommon
%include "std_shared_ptr.i"
%include "std_string.i"
%include "std_vector.i"
%shared_ptr(_VNCommonLabel)
%{
#include "text_util.h"
#include "vncommon.h"
%}
namespace std {
   %template(StringVector) vector<std::string>;
}

/*std::string normalized_date(std::string text);*/
/*std::string normalized_number(std::string text);*/
/*std::string normalized_time(std::string text);*/
/*std::string normalized_unit(std::string text);*/
%include "text_util.h"
%include "vncommon.h"
