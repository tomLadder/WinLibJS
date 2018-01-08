#define DRIVER_CPP

#include <node.h>
#include <nan.h>
#include <Driver.h>

//WinLib
using WinLib::PE::Loader::Driver;

//Google V8 | Nan
using v8::Isolate;
using v8::HandleScope;
using v8::Exception;
using v8::String;
using v8::Function;
using v8::Array;
using v8::Number;
using v8::Local;
using v8::Value;
using v8::Null;
using v8::Object;
using v8::Boolean;

using Nan::Callback;
using Nan::New;
using Nan::AsyncQueueWorker;
using Nan::AsyncWorker;

class DriverLoadWorker: public AsyncWorker {
private:
  std::string path;
  bool result;
public:
  DriverLoadWorker(Callback *callback, std::string path)
    : AsyncWorker(callback), path(path) {}

  ~DriverLoadWorker() {}

  void Execute () {
    auto driver = new Driver((char*)path.c_str());

    this->result = driver->load();

    delete driver;
  }

  // We have the results, and we're back in the event loop.
  void HandleOKCallback () {
    Isolate* isolate = Isolate::GetCurrent();
    Nan:: HandleScope scope;

    Local<Value> argv[] = { New<Boolean>(this->result) };
    callback->Call(1, argv);
  }
};

NAN_METHOD(LoadDriver) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  if(!info[0]->IsString()) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Argument 1 must be a string (path)!")));
    return;
  }

  if(!info[1]->IsFunction()) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Argument 2 muste be a function (err)!")));
    return;
  }

  //Param parse
  v8::String::Utf8Value _path(info[0]->ToString());
  std::string path = std::string(*_path);
  Callback* errCallback = new Callback(info[1].As<Function>());
  
  AsyncQueueWorker(new DriverLoadWorker(errCallback, path));
}

NAN_MODULE_INIT(Initialize) {
  NAN_EXPORT(target, LoadDriver);
}

NODE_MODULE(addon, Initialize);