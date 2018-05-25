#include <node_api.h>
#include <exception>
#include "addon.h"

#define napiErrChk(status) \
  if (status != napi_ok){ \
    const napi_extended_error_info* err_info_ptr; \
    napi_get_last_error_info(env, &err_info_ptr); \
    throw std::runtime_error(err_info_ptr->error_message); \
  }

void viErrChk(ViObject vi, ViStatus vi_status){
  if (vi_status != VI_SUCCESS){
    ViChar ErrorMsg[256];
    if (viStatusDesc(vi, vi_status, ErrorMsg) != VI_SUCCESS){
      throw std::runtime_error("Unknown VI error");
    } else {
      throw std::runtime_error(ErrorMsg);
    }
  }
}

#define processRuntimeError(deferred) \
  napi_value error, error_code, error_msg; \
  napi_create_string_utf8(env, "ERROR", NAPI_AUTO_LENGTH, &error_code); \
  napi_create_string_utf8(env, e.what(), NAPI_AUTO_LENGTH, &error_msg); \
  napi_create_error(env, error_code, error_msg, &error); \
  napi_reject_deferred(env, deferred, error);

napi_value create_promise(napi_env env, napi_deferred& deferred, napi_async_execute_callback execute, napi_async_complete_callback complete, void* pArgArr){
  napi_value promise;
  napiErrChk(napi_create_promise(env, &deferred, &promise));
  
  napi_async_work asyncWorkResult;
  napiErrChk(napi_create_async_work(env, NULL, NULL, execute, complete, pArgArr, &asyncWorkResult));
  napiErrChk(napi_queue_async_work(env, asyncWorkResult));
  return promise;
}

/*=============================================================*
* FUNCTION: viOpenDefaultRM
*==============================================================*/
napi_deferred deferred_viOpenDefaultRM;

void execute_viOpenDefaultRM(napi_env env, void* data){
  arg_viOpenDefaultRM* _data = (arg_viOpenDefaultRM*)data;
  _data->status = viOpenDefaultRM(&(_data->vi));
}

void complete_viOpenDefaultRM(napi_env env, napi_status status, void* data){
  try {
    napiErrChk(status);
    arg_viOpenDefaultRM* _data = (arg_viOpenDefaultRM*)data;
    viErrChk(_data->vi, _data->status);
    napi_value result;  
    napiErrChk(napi_create_uint32(env, _data->vi, &result));
    napiErrChk(napi_resolve_deferred(env, deferred_viOpenDefaultRM, result));
  } catch(const std::runtime_error& e){
    processRuntimeError(deferred_viOpenDefaultRM); // promise is rejected with e.what()
  }
  deferred_viOpenDefaultRM = NULL;
}

napi_value viOpenDefaultRM(napi_env env, const napi_callback_info info){
  static arg_viOpenDefaultRM argArr;
  try {
    size_t argc = 1;
    napi_value args[1];
    napiErrChk(napi_get_cb_info(env, info, &argc, args, nullptr, nullptr));

/*     napiErrChk(napi_get_value_double(env, args[0], &(argArr.input1)));
    napiErrChk(napi_get_value_double(env, args[1], &(argArr.input2))); */
    
    return create_promise(env, deferred_viOpenDefaultRM, execute_viOpenDefaultRM, complete_viOpenDefaultRM, &argArr);
  } catch(const std::runtime_error&){
    return NULL;
  }
}

/*=============================================================*
* FUNCTION: viClose
*==============================================================*/
napi_deferred deferred_viClose;

void execute_viClose(napi_env env, void* data){
  arg_viClose* _data = (arg_viClose*)data;
  _data->status = viClose(_data->vi);
}

void complete_viClose(napi_env env, napi_status status, void* data){
  try {
    napiErrChk(status);
    arg_viClose* _data = (arg_viClose*)data;
    viErrChk(_data->vi, _data->status);
    napi_value result;  
    napiErrChk(napi_get_undefined(env, &result));
    napiErrChk(napi_resolve_deferred(env, deferred_viClose, result));
  } catch(const std::runtime_error& e){
    processRuntimeError(deferred_viClose);
  }
  deferred_viClose = NULL;
}

napi_value viClose(napi_env env, const napi_callback_info info){
  static arg_viClose argArr;
  try {
    size_t argc = 1;
    napi_value args[1];
    napiErrChk(napi_get_cb_info(env, info, &argc, args, nullptr, nullptr));

    uint32_t tempUint32;
    napiErrChk(napi_get_value_uint32(env, args[0], &tempUint32));
    argArr.vi = (ViObject)tempUint32;
    
    return create_promise(env, deferred_viClose, execute_viClose, complete_viClose, &argArr);
  } catch(const std::runtime_error&){
    return NULL;
  }
}

/*=============================================================*
* FUNCTION: viOpen
*==============================================================*/
napi_deferred deferred_viOpen;

void execute_viOpen(napi_env env, void* data){
  arg_viOpen* _data = (arg_viOpen*)data;
  _data->status = viOpen(_data->sesn, _data->name, _data->mode, _data->timeout, &(_data->vi));
}

void complete_viOpen(napi_env env, napi_status status, void* data){
  try {
    napiErrChk(status);
    arg_viOpen* _data = (arg_viOpen*)data;
    viErrChk(_data->vi, _data->status);
    napi_value result;  
    napiErrChk(napi_create_uint32(env, _data->vi, &result));
    napiErrChk(napi_resolve_deferred(env, deferred_viOpen, result));
  } catch(const std::runtime_error& e){
    processRuntimeError(deferred_viOpen);
  }
  deferred_viOpen = NULL;
}

napi_value viOpen(napi_env env, const napi_callback_info info){
  static arg_viOpen argArr;
  try {
    size_t argc = 4;
    napi_value args[4];
    napiErrChk(napi_get_cb_info(env, info, &argc, args, nullptr, nullptr));

    uint32_t tempUint32;
    napiErrChk(napi_get_value_uint32(env, args[0], &tempUint32));
    argArr.sesn = (ViSession)tempUint32;
    size_t tempSize;
    napiErrChk(napi_get_value_string_utf8(env, args[1], argArr.name, 128, &tempSize));
    
    if (argc > 2){
      napiErrChk(napi_get_value_uint32(env, args[2], &tempUint32));
      argArr.mode = (ViAccessMode)tempUint32;
    } else {
      argArr.mode = VI_NULL;
    }
    
    if (argc > 3){
      napiErrChk(napi_get_value_uint32(env, args[3], &tempUint32));
      argArr.timeout = (ViUInt32)tempUint32;
    } else {
      argArr.timeout = VI_NULL;
    }      
    
    return create_promise(env, deferred_viOpen, execute_viOpen, complete_viOpen, &argArr);
  } catch(const std::runtime_error&){
    return NULL;
  }
}

/*=============================================================*
* FUNCTION: viPrintf
*==============================================================*/
napi_deferred deferred_viPrintf;

void execute_viPrintf(napi_env env, void* data){
  arg_viPrintf* _data = (arg_viPrintf*)data;
  _data->status = viPrintf(_data->vi, _data->writeFmt);
}

void complete_viPrintf(napi_env env, napi_status status, void* data){
  try {
    napiErrChk(status);
    arg_viPrintf* _data = (arg_viPrintf*)data;
    viErrChk(_data->vi, _data->status);
    napi_value result;  
    napiErrChk(napi_get_undefined(env, &result));
    napiErrChk(napi_resolve_deferred(env, deferred_viPrintf, result));
  } catch(const std::runtime_error& e){
    processRuntimeError(deferred_viPrintf);
  }
  deferred_viPrintf = NULL;
}

napi_value viPrintf(napi_env env, const napi_callback_info info){
  static arg_viPrintf argArr;
  try {
    size_t argc = 2;
    napi_value args[2];
    napiErrChk(napi_get_cb_info(env, info, &argc, args, nullptr, nullptr));

    uint32_t tempUint32;
    napiErrChk(napi_get_value_uint32(env, args[0], &tempUint32));
    argArr.vi = (ViSession)tempUint32;
    size_t tempSize;
    napiErrChk(napi_get_value_string_utf8(env, args[1], argArr.writeFmt, 128, &tempSize));
    
    return create_promise(env, deferred_viPrintf, execute_viPrintf, complete_viPrintf, &argArr);
  } catch(const std::runtime_error&){
    return NULL;
  }
}

/*=============================================================*
* FUNCTION: viScanf
*==============================================================*/
napi_deferred deferred_viScanf;

void execute_viScanf(napi_env env, void* data){
  arg_viScanf* _data = (arg_viScanf*)data;
  _data->output.assign("\0", 100000);
  _data->status = viScanf(_data->vi, _data->readFmt, _data->output.data());
}

void complete_viScanf(napi_env env, napi_status status, void* data){
  try {
    napiErrChk(status);
    arg_viScanf* _data = (arg_viScanf*)data;
    viErrChk(_data->vi, _data->status);
    napi_value result;  
    napiErrChk(napi_create_string_utf8(env, _data->output.data(), NAPI_AUTO_LENGTH, &result));
    napiErrChk(napi_resolve_deferred(env, deferred_viScanf, result));
  } catch(const std::runtime_error& e){
    processRuntimeError(deferred_viScanf);
  }
  deferred_viScanf = NULL;
}

napi_value viScanf(napi_env env, const napi_callback_info info){
  static arg_viScanf argArr;
  try {
    size_t argc = 2;
    napi_value args[2];
    napiErrChk(napi_get_cb_info(env, info, &argc, args, nullptr, nullptr));

    uint32_t tempUint32;
    napiErrChk(napi_get_value_uint32(env, args[0], &tempUint32));
    argArr.vi = (ViSession)tempUint32;
    
    if (argc > 1){
      size_t tempSize;
      napiErrChk(napi_get_value_string_utf8(env, args[1], argArr.readFmt, 128, &tempSize));
    } else {
      strcpy(argArr.readFmt, "%[^\n]");
    }
    
    return create_promise(env, deferred_viScanf, execute_viScanf, complete_viScanf, &argArr);
  } catch(const std::runtime_error&){
    return NULL;
  }
}

#define DECLARE_NAPI_METHOD(name) \
  {#name, 0, name, 0, 0, 0, napi_default, 0}
  
napi_value Init(napi_env env, napi_value exports){
  napi_status status;
  napi_property_descriptor descriptors[] = {
    DECLARE_NAPI_METHOD(viOpenDefaultRM),
    DECLARE_NAPI_METHOD(viClose),
    DECLARE_NAPI_METHOD(viOpen),
    DECLARE_NAPI_METHOD(viPrintf),
    DECLARE_NAPI_METHOD(viScanf)
  };
  status = napi_define_properties(env, exports, sizeof(descriptors) / sizeof(descriptors[0]), descriptors);
  return exports;
}

NAPI_MODULE(addon, Init)