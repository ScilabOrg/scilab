#include "ReadLine.hxx"

namespace org_scilab_modules_gui_bridge {

  char * ReadLine::readLine (JavaVM * jvm_)
  {
    JNIEnv * curEnv = NULL;
    jvm_->AttachCurrentThread(reinterpret_cast<void **>(&curEnv), NULL);
    jclass cls = curEnv->FindClass(className().c_str());

    jmethodID jstringreadLineID = curEnv->GetStaticMethodID(cls, "readLine", "()Ljava/lang/String;");
    if (jstringreadLineID == NULL)
    {
	jvm_->DetachCurrentThread();
	throw GiwsException::JniMethodNotFoundException(curEnv, "readLine");
    }

    jstring res = static_cast<jstring>(curEnv->CallStaticObjectMethod(cls, jstringreadLineID));
    if (curEnv->ExceptionCheck())
    {
	jvm_->DetachCurrentThread();
	throw GiwsException::JniCallMethodException(curEnv);
    }

    const char *tempString = curEnv->GetStringUTFChars(res, 0);
    char * myStringBuffer = new char[strlen(tempString) + 1];
    strcpy(myStringBuffer, tempString);
    curEnv->ReleaseStringUTFChars(res, tempString);
    curEnv->DeleteLocalRef(res);
    if (curEnv->ExceptionCheck())
    {
	delete[] myStringBuffer;
	jvm_->DetachCurrentThread();
	throw GiwsException::JniCallMethodException(curEnv);
    }

    jvm_->DetachCurrentThread();

    return myStringBuffer;
  }
}
