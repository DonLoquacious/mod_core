%include ../../../../swig_common.i
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;
typedef unsigned long long uint64_t;
typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;
typedef unsigned long in_addr_t;

%typemap(imtype, out="string") char **   "ref string"
%typemap(cstype, out="string") char **   "ref string"
%typemap(csin) char **     "ref $csinput"
%typemap(csvarin) char **
%{
  set { $imcall; }
%}
%typemap(csvarout) char **
%{
  get {
    return $imcall;
  }
%}

// const char ** -> out string
%typemap(imtype, out="string") const char ** "ref global::System.IntPtr"
%typemap(cstype, out="string") const char ** "out string"
%typemap(csin,
        pre="var $csinput_ptr = global::System.IntPtr.Zero;",
        post="if($csinput_ptr != global::System.IntPtr.Zero)\n"
             "\t$csinput = global::System.Runtime.InteropServices.Marshal.PtrToStringAnsi($csinput_ptr);\n"
             "else\n"
             "\t$csinput = null;"
) const char ** "ref $csinput_ptr"

%newobject EventConsumer::pop;
%newobject Session;
%newobject CoreSession;
%newobject Event;
%newobject Stream;
%newobject API::execute;
%newobject API::executeString;
%newobject CoreSession::playAndDetectSpeech;

#define SWITCH_DECLARE(type) type
#define SWITCH_DECLARE_NONSTD(type) type
#define SWITCH_MOD_DECLARE(type) type
#define SWITCH_MOD_DECLARE_NONSTD(type) type
#define SWITCH_DECLARE_DATA
#define SWITCH_MOD_DECLARE_DATA
#define SWITCH_THREAD_FUNC
#define SWITCH_DECLARE_CONSTRUCTOR SWITCH_DECLARE_DATA

#define _In_
#define _In_z_
#define _In_opt_z_
#define _In_opt_
#define _Printf_format_string_
#define _Ret_opt_z_
#define _Ret_z_
#define _Out_opt_
#define _Out_
#define _Check_return_
#define _Inout_
#define _Inout_opt_
#define _In_bytecount_(x)
#define _Out_opt_bytecapcount_(x)
#define _Out_bytecapcount_(x)
#define _Ret_
#define _Post_z_
#define _Out_cap_(x)
#define _Out_z_cap_(x)
#define _Out_ptrdiff_cap_(x)
#define _Out_opt_ptrdiff_cap_(x)
#define _Post_count_(x)