#include <nan.h>
#include <node.h>
#include <MMap.h>
#include <LoadLibInjection.h>
#include <Console.h>

//WinLib
using WinLib::PE::Loader::LoadLibInjection;
using WinLib::PE::Loader::MMapper;
using WinLib::PE::PEFile;
using WinLib::Output::Console;
using WinLib::Output::LogType;


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

using Nan::Callback;
using Nan::New;
using Nan::AsyncQueueWorker;
using Nan::AsyncWorker;

class MMapWorker : public AsyncWorker {
private:
  int pid;
  std::string path;
  MMapper::STATUS status;
public:
    MMapWorker(Callback *callback, int pid, std::string path)
      : AsyncWorker(callback), pid(pid), path(path) {}

    ~MMapWorker() {}


    void Execute () {
      auto pe = PEFile::loadFromFile(path);
      if (pe) {
        MMapper* mapper = new MMapper(pe);

        this->status = mapper->map(pid);
      }
    }

    // We have the results, and we're back in the event loop.
    void HandleOKCallback () {
      Isolate* isolate = Isolate::GetCurrent();
      Nan:: HandleScope scope;

      //Parse enum
      //Local<Object> obj;
      //obj->DefineOwnProperty(isolate->GetCurrentContext(),String::NewFromUtf8(isolate,"enum"),Number::New(isolate,this->status), v8::ReadOnly);

      Local<Value> argv[] = { New<Number>(this->status) };
      callback->Call(1, argv);
    }
};

NAN_METHOD(ManualMap) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  if(!info[0]->IsNumber()) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Argument 1 must be a number (ProcessID)!")));
    return;
  }

  if(!info[1]->IsString()) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Argument 2 must be a string (path)!")));
    return;
  }

  if(!info[2]->IsFunction()) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Argument 3 muste be a function (err)!")));
    return;
  }

  //Param parse
  int pid = (int) info[0]->NumberValue();
  v8::String::Utf8Value _path(info[1]->ToString());
  std::string path = std::string(*_path);
  Callback* errCallback = new Callback(info[2].As<Function>());
  
  AsyncQueueWorker(new MMapWorker(errCallback, pid, path));
}

NAN_MODULE_INIT(Initialize) {
  NAN_EXPORT(target, ManualMap);
}

NODE_MODULE(addon, Initialize);