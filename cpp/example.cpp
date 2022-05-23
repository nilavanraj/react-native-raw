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

std::string base64_decode(std::string const& encoded_string) {
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;
    while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i == 4) {
            for (i = 0; i < 4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 4; j++)
            char_array_4[j] = 0;

        for (j = 0; j < 4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }

    return ret;
}


void base64_decode_image(const string& input,const string& path) {
    ofstream outfile;
    outfile.open(path, ios::binary);
    string temp = base64_decode(input);
   if(outfile.write(temp.c_str(), temp.size()))
    __android_log_print(ANDROID_LOG_VERBOSE, "LOG_TAG","created"  );

    outfile.close();
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

 auto write = Function::createFromHostFunction(jsiRuntime,
                                                       PropNameID::forAscii(jsiRuntime,
                                                                            "write"),
                                                       0,
                                                       [](Runtime &runtime,
                                                          const Value &thisValue,
                                                          const Value *arguments,
                                                          size_t count) -> Value {


    string x = arguments[0].getString(runtime).utf8(runtime);
    string path = arguments[0].getString(runtime).utf8(runtime);

    base64_decode_image(x,path);

  //  __android_log_print(ANDROID_LOG_VERBOSE, "LOG_EGG","%s\n", path.c_str() );

        return Value(runtime,
                     String::createFromUtf8(
                                            runtime,
                                            "created"));

    });


    jsiRuntime.global().setProperty(jsiRuntime, "write", move(write));


    auto foo = Function::createFromHostFunction(
            jsiRuntime,
            PropNameID::forAscii(jsiRuntime, "foo"),
            1,
            [](Runtime& runtime, const Value& thisValue, const Value* arguments, size_t count) -> Value {

                auto userCallbackRef = std::make_shared<Object>(arguments[1].getObject(runtime));
                string x = arguments[0].getString(runtime).utf8(runtime);

                string readSync =ReadFileByBlocks(x.c_str());
                    __android_log_print(ANDROID_LOG_VERBOSE, "LOG_TAG","%s\n", arguments[0].getString(runtime).utf8(runtime).c_str()  );

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
    jsiRuntime.global().setProperty(jsiRuntime, "foo", std::move(foo));
}

}
