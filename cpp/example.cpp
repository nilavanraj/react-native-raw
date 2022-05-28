#include "example.h"
#include <jsi/jsi.h>
#include<thread>
#include <fstream>
#include <android/log.h>


using namespace facebook::jsi;
using namespace std;

static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

static const unsigned char base64_table[65] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


static inline bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}
static const int B64index[256] = { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                                   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
                                   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 62, 63, 62, 62, 63, 52, 53, 54, 55,
                                   56, 57, 58, 59, 60, 61,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  6,
                                   7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  0,
                                   0,  0,  0, 63,  0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
                                   41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51 };

std::string base64_encode(const unsigned char *src, size_t len)
{
    unsigned char *out, *pos;
    const unsigned char *end, *in;

    size_t olen;

    olen = 4*((len + 2) / 3); /* 3-byte blocks to 4-byte */

    if (olen < len)
        return std::string(); /* integer overflow */

    std::string outStr;
    outStr.resize(olen);
    out = (unsigned char*)&outStr[0];

    end = src + len;
    in = src;
    pos = out;
    while (end - in >= 3) {
        *pos++ = base64_table[in[0] >> 2];
        *pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
        *pos++ = base64_table[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
        *pos++ = base64_table[in[2] & 0x3f];
        in += 3;
    }

    if (end - in) {
        *pos++ = base64_table[in[0] >> 2];
        if (end - in == 1) {
            *pos++ = base64_table[(in[0] & 0x03) << 4];
            *pos++ = '=';
        }
        else {
            *pos++ = base64_table[((in[0] & 0x03) << 4) |
                (in[1] >> 4)];
            *pos++ = base64_table[(in[1] & 0x0f) << 2];
        }
        *pos++ = '=';
    }

    return outStr;
}

std::string base64_decode(const void* data, const size_t len)
{
    unsigned char* p = (unsigned char*)data;
    int pad = len > 0 && (len % 4 || p[len - 1] == '=');
    const size_t L = ((len + 3) / 4 - pad) * 4;
    std::string str(L / 4 * 3 + pad, '\0');

    for (size_t i = 0, j = 0; i < L; i += 4)
    {
        int n = B64index[p[i]] << 18 | B64index[p[i + 1]] << 12 | B64index[p[i + 2]] << 6 | B64index[p[i + 3]];
        str[j++] = n >> 16;
        str[j++] = n >> 8 & 0xFF;
        str[j++] = n & 0xFF;
    }
    if (pad)
    {
        int n = B64index[p[L]] << 18 | B64index[p[L + 1]] << 12;
        str[str.size() - 1] = n >> 16;

        if (len > L + 2 && p[L + 2] != '=')
        {
            n |= B64index[p[L + 2]] << 6;
            str.push_back(n >> 8 & 0xFF);
        }
    }
    return str;
}


std::string  base64_decode_image(const string& input,const string& path) {
    ofstream outfile;
    outfile.open(path, ios::binary);
    string temp = base64_decode(input.c_str(),input.size());
    outfile.write(temp.c_str(), temp.size());
    outfile.close();
    if(outfile)
        return "File Created";
    else
        return "Failed to Create File";


   // __android_log_print(ANDROID_LOG_VERBOSE, "LOG_TAG","%s\n", input.c_str()  );
}

string base64_encode_image(const string& path) {
    vector<char> temp;

    std::ifstream infile;
    infile.open(path,ios_base::out | std::ifstream::binary);
    if (infile.is_open()) {
        while (!infile.eof()) {
            char c = (char)infile.get();
            temp.push_back(c);
        }
        infile.close();
    }
    else {
        return "File could not be opened";
        }

    string ret(temp.begin(), temp.end() - 1);
    ret = base64_encode((unsigned const char*)ret.c_str(), ret.size());

    return ret;
}

string ReadFileByBlocks(const char* filename)
{
    vector<string> vecstr;
        size_t buffer_size = 1024 * 1024;
        char* buffer = new char[buffer_size];
        // std::ifstream::binary | std::ifstream::ate
        std::ifstream fin(filename, ifstream::binary);

        while (fin)
        {
            fin.read(buffer, buffer_size);
            size_t count = fin.gcount();
            if (!count)
                break;
            string w(buffer, fin.gcount());
            vecstr.push_back(w);
        }
        std::string ret;
        for (std::vector<std::string>::const_iterator i = vecstr.begin(); i != vecstr.end(); ++i)
            ret += *i;
        ret = base64_encode((unsigned const char*)ret.c_str(), ret.size());
    return ret;
}

namespace example {

void install(Runtime &jsiRuntime) {
 auto readSync = Function::createFromHostFunction(jsiRuntime,
                                                       PropNameID::forAscii(jsiRuntime,
                                                                            "readSync"),
                                                       0,
                                                       [](Runtime &runtime,
                                                          const Value &thisValue,
                                                          const Value *arguments,
                                                          size_t count) -> Value {

        string x = arguments[0].getString(runtime).utf8(runtime);
        string readSync = ReadFileByBlocks(x.c_str());
        return Value(runtime,
                     String::createFromUtf8(
                                            runtime,
                                            readSync));

    });


    jsiRuntime.global().setProperty(jsiRuntime, "readSync", move(readSync));

 auto writeSync = Function::createFromHostFunction(jsiRuntime,
                                                       PropNameID::forAscii(jsiRuntime,
                                                                            "writeSync"),
                                                       0,
                                                       [](Runtime &runtime,
                                                          const Value &thisValue,
                                                          const Value *arguments,
                                                          size_t count) -> Value {


    string x = arguments[0].getString(runtime).utf8(runtime);
    string path = arguments[1].getString(runtime).utf8(runtime);

    string result = base64_decode_image(x,path);

  //  __android_log_print(ANDROID_LOG_VERBOSE, "LOG_EGG","%s\n", path.c_str() );

        return Value(runtime,
                     String::createFromUtf8(
                                            runtime,
                                            result));

    });


    jsiRuntime.global().setProperty(jsiRuntime, "writeSync", move(writeSync));


    auto readAsync = Function::createFromHostFunction(
            jsiRuntime,
            PropNameID::forAscii(jsiRuntime, "readAsync"),
            1,
            [](Runtime& runtime, const Value& thisValue, const Value* arguments, size_t count) -> Value {

                auto userCallbackRef = std::make_shared<Object>(arguments[1].getObject(runtime));
                string x = arguments[0].getString(runtime).utf8(runtime);
                string readSync =ReadFileByBlocks(x.c_str());


                auto f = [&runtime](shared_ptr<Object> userCallbackRef, string readSync) {

                    auto val = String::createFromUtf8(runtime, readSync);
                    auto error = Value::undefined();
                    userCallbackRef->asFunction(runtime).call(runtime, error, val);
                };

                std::thread thread_object(f,userCallbackRef,readSync);
                thread_object.detach();

                return Value::undefined();
            }
    );
    jsiRuntime.global().setProperty(jsiRuntime, "readAsync", std::move(readAsync));

    auto writeAsync = Function::createFromHostFunction(
            jsiRuntime,
            PropNameID::forAscii(jsiRuntime, "writeAsync"),
            1,
            [](Runtime& runtime, const Value& thisValue, const Value* arguments, size_t count) -> Value {

                auto userCallbackRef = std::make_shared<Object>(arguments[2].getObject(runtime));
                string x = arguments[0].getString(runtime).utf8(runtime);
                string path = arguments[1].getString(runtime).utf8(runtime);


                auto f = [&runtime](shared_ptr<Object> userCallbackRef, string x,string path) {
                    string result = base64_decode_image(x,path);
                    auto val = String::createFromUtf8(runtime, result);
                    auto error = Value::undefined();
                    userCallbackRef->asFunction(runtime).call(runtime, error, val);
                };

                std::thread thread_object(f,userCallbackRef,x,path);
                thread_object.detach();

                return Value::undefined();
            }
    );
    jsiRuntime.global().setProperty(jsiRuntime, "writeAsync", std::move(writeAsync));
}

}
