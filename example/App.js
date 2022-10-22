/* eslint-disable prettier/prettier */
/* eslint-disable no-undef */

import * as React from 'react';
import {
  StyleSheet,
  View,
  Text,
  TouchableOpacity,
  Image,
} from 'react-native';
import rawJsiModule, {isLoaded} from 'react-native-raw';
import FetchBlob from 'rn-fetch-blob';

export default function App() {
  const [ImageBase64, setAsyncJsi] = React.useState();
  const [avg, setAvg] = React.useState({
    syncJsi: 0,
    asyncJsi: 0,
    RNFetchBlob: 0,
    writeSync:0,
    writeAsync:0,
    syncJsiNo: 0,
    asyncJsiNo: 0,
    RNFetchBlobNo: 0,
    writeSyncNo:0,
    writeAsyncNo:0,
    RNFetchBlobWrite:0,
    RNFetchBlobWriteNo:0
  });

const downloadURL = 'https://buffer.com/cdn-cgi/image/w=1000,fit=contain,q=90,f=auto/library/content/images/size/w600/2020/09/Frame-47.png';
const filename = '/sync11.png';
const base64Icon = 'data:image/png;base64,' + ImageBase64;

//  const  {
//     syncJsi,
//     asyncJsi,
//     RNFetchBlob,
//     writeSync,
//     writeAsync,
//     syncJsiNo,
//     asyncJsiNo,
//     RNFetchBlobNo,
//     writeSyncNo,
//     writeAsyncNo,
//     RNFetchBlobWrite,
//     RNFetchBlobWriteNo
//   } = avg;
//   console.log('--------------------------------------------------------------------');
//   console.log('Read sync  Avg time:',syncJsi / syncJsiNo,"Number of test:",syncJsiNo);
//   console.log('Read async  Avg time:',asyncJsi / asyncJsiNo,"Number of test:",asyncJsiNo);
//   console.log('write Sync Avg time:',writeSync / writeSyncNo,"Number of test:",writeSyncNo);
//   console.log('write Async  Avg time:',writeAsync / writeAsyncNo,"Number of test:",writeAsyncNo);
//   console.log('RNFetchBlob Read Avg time:',RNFetchBlob / RNFetchBlobNo,"Number of test:",RNFetchBlobNo);
//   console.log('RNFetchBlob Write Avg time:',RNFetchBlobWrite / RNFetchBlobWriteNo,"Number of test:",RNFetchBlobWriteNo);
//   console.log('--------------------------------------------------------------------');

  return (
    <View style={styles.container}>
      <Text>Bindings Installed: {isLoaded().toString()}</Text>
      {/* readSync */}
      <TouchableOpacity
        onPress={() => {
          const t1 = new Date();
          const now = rawJsiModule.readSync(
            rawJsiModule.Dir.PictureDir + filename,
          );
          const t2 = new Date();
        // console.log(t2 - t1,now);
         setAsyncJsi(now);

          setAvg(preState => {
            return {
              ...preState,
              syncJsi: preState.syncJsi + (t2 - t1),
              syncJsiNo:preState.syncJsiNo + 1,
            };
          });
        }}
        style={styles.button}>
        <Text style={styles.buttonTxt}>readSync</Text>
      </TouchableOpacity>

       {/* readAsync */}
      <TouchableOpacity
        onPress={() => {
          const t1 = new Date();
          rawJsiModule.readAsync(
            rawJsiModule.Dir.PictureDir + filename,
            (error, result) => {
              const t2 = new Date();
               setAsyncJsi(result);
              setAvg(preState => {
                return {
                  ...preState,
                  asyncJsi: (preState.asyncJsi + (t2 - t1)) ,
                  asyncJsiNo:preState.asyncJsiNo + 1,
                };
              });
            },
          );
        }}
        style={styles.button}>
        <Text style={styles.buttonTxt}>readAsync</Text>
      </TouchableOpacity>

      {/* writeSync */}
      <TouchableOpacity
        onPress={() => {
          const t1 = new Date();
          fetch(
            downloadURL,
          )
            .then(response => response.blob())
            .then(blob => {
              var reader = new FileReader();
              reader.onload = function () {
                const test = this.result.split(',');
                const t1 = new Date();
               const now = rawJsiModule.writeSync(
                  test[1],
                  rawJsiModule.Dir.PictureDir + filename,
                );
                const t2 = new Date();
                setAvg(preState => {
                  return {
                    ...preState,
                    writeSync: (preState.writeSync + (t2 - t1)),
                    writeSyncNo:preState.writeSyncNo + 1,
                  };
                });
              };
              reader.readAsDataURL(blob);
            });
        }}
        style={styles.button}>
        <Text style={styles.buttonTxt}>writeSync</Text>
      </TouchableOpacity>

      {/* writeAsync */}
      <TouchableOpacity
        onPress={() => {
          const t1 = new Date();
          fetch(
            downloadURL,
          )
            .then(response => response.blob())
            .then(blob => {
              var reader = new FileReader();
              reader.onload = function () {
                const test = this.result.split(',');
                const t1 = new Date();
                rawJsiModule.writeAsync(
                  test[1],
                  rawJsiModule.Dir.PictureDir + filename,
                  (error,data)=>{

                    const t2 = new Date();
                    setAvg(preState => {
                      return {
                        ...preState,
                        writeAsync: (preState.writeAsync + (t2 - t1)),
                        writeAsyncNo:preState.writeAsyncNo + 1,
                      };
                    });
                  }
                );
              };
              reader.readAsDataURL(blob);
            });
        }}
        style={styles.button}>
        <Text style={styles.buttonTxt}>writeAsync</Text>
      </TouchableOpacity>

      {/* RN fetch blob Read*/}
      <TouchableOpacity
        onPress={() => {
          const t1 = new Date();
          FetchBlob.fs
            .readFile(
              rawJsiModule.Dir.PictureDir + filename,
              'base64',
            )
            .then(set => {
              const t2 = new Date();
              setAsyncJsi(set);
             // console.log(t2 - t1);
              // setrn(set);
              setAvg(preState => {
                return {
                  ...preState,
                  RNFetchBlob: (preState.RNFetchBlob + (t2 - t1)),
                  RNFetchBlobNo:preState.RNFetchBlobNo + 1,
                };
              });
            });
        }}
        style={styles.button}>
        <Text style={styles.buttonTxt}>RN fetch blob Read</Text>
      </TouchableOpacity>

      {/* RN fetch blob Write*/}
      <TouchableOpacity
        onPress={() => {
          fetch(
            downloadURL,
          )
            .then(response => response.blob())
            .then(blob => {
              var reader = new FileReader();
              reader.onload = function () {
                const test = this.result.split(',');
                const t1 = new Date();
                  FetchBlob.fs.writeStream(
                    rawJsiModule.Dir.PictureDir + filename,
                    // encoding, should be one of `base64`, `utf8`, `ascii`
                    'base64',
                    // should data append to existing content ?
                    true
                )
                .then(ofstream => ofstream.write(test[1]))
                .then(ofstream => ofstream.close())
                .then(() => {
                  const t2 = new Date();
                  setAvg(preState => {
                    return {
                      ...preState,
                      RNFetchBlobWrite: (preState.RNFetchBlobWrite + (t2 - t1)),
                      RNFetchBlobWriteNo:preState.RNFetchBlobWriteNo + 1,
                    };
                  });
                })
                .catch(console.error);
              };
              reader.readAsDataURL(blob);
            });
        }}
        style={styles.button}>
        <Text style={styles.buttonTxt}>RN fetch blob Write</Text>
      </TouchableOpacity>

      <TouchableOpacity
        onPress={() => {
          setAsyncJsi(null);
        }}
        style={styles.button}>
        <Text style={styles.buttonTxt}>reset</Text>
      </TouchableOpacity>
      {/* write Concurrent */}
      <TouchableOpacity
        onPress={() => {
       
              rawJsiModule.writeConcurrent(
                "",
                rawJsiModule.Dir.PictureDir + filename,
                0,
                (error,data)=>{
                  console.log(error,data)
                 
                  });

                  rawJsiModule.writeConcurrent(
                    "",
                    rawJsiModule.Dir.PictureDir + filename,
                    7545,
                    (error,data)=>{
                      console.log(error,data)
                     
                      });
            }}
        style={styles.button}>
        <Text style={styles.buttonTxt}>write Concurrent</Text>
      </TouchableOpacity>

      {ImageBase64 ? (
        <Image
          style={{
            width: 100,
            height: 50,
            resizeMode: 'contain',
            borderWidth: 1,
            borderColor: 'red',
          }}
          source={{uri: base64Icon}}
        />
      ) : (
        <></>
      )}
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center',
  },
  box: {
    width: 60,
    height: 60,
    marginVertical: 20,
  },
  button: {
    width: '95%',
    alignSelf: 'center',
    height: 40,
    backgroundColor: 'green',
    alignItems: 'center',
    justifyContent: 'center',
    borderRadius: 5,
    marginVertical: 10,
  },
  buttonTxt: {
    color: 'white',
  },
});
