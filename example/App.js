import * as React from 'react';
import {
  StyleSheet,
  View,
  Text,
  TouchableOpacity,
  NativeModules,
  Image,
} from 'react-native';
import simpleJsiModule, {isLoaded} from 'react-native-raw';
import RNFetchBlob from 'rn-fetch-blob';

export default function App() {
  const [result, setResult] = React.useState();
  const [asyncJsi, setAsyncJsi] = React.useState();
  const [avg, setAvg] = React.useState({
    syncJsi: 0,
    asyncJsi: 0,
    RNFetchBlob: 0,
  });

  const [rn, setrn] = React.useState();
  //console.log(simpleJsiModule.Dir);
  React.useEffect(() => {
    setResult('hhh');
    fetch(
      'https://s3.ap-south-1.amazonaws.com/leaguex/team-images/bblw/MLSW.png',
    )
      .then(response => response.blob())
      .then(blob => {
        var reader = new FileReader();
        reader.onload = function () {
          const test = this.result.split(',');
          const t1 = new Date();
          simpleJsiModule.write(
            test[1],
            '/storage/emulated/0/Android/data/com.example/files/Pictures/sync.png',
          );
          const t2 = new Date();
          console.log(t2 - t1);

          const t3 = new Date();
          RNFetchBlob.fs
            .writeStream(
              '/storage/emulated/0/Android/data/com.example/files/Pictures/sync.png',
              'base64',
              true,
            )
            .then(ofstream => ofstream.write(test[1]))
            .then(ofstream => {
              const t4 = new Date();
              console.log(t4 - t3);
            })
            .then(ofstream => ofstream.close())
            .catch(console.error);
        };
        reader.readAsDataURL(blob);
      });
  }, []);
  var base64Icon = 'data:image/png;base64,' + asyncJsi;
console.log(avg);

  return (
    <View style={styles.container}>
      <Text>Bindings Installed: {isLoaded().toString()}</Text>
      <Text>Result: {result}</Text>

      <TouchableOpacity
        onPress={() => {
          const t1 = new Date();
          const now = simpleJsiModule.readSync(
            '/storage/emulated/0/Android/data/com.example/files/Pictures/IMG20220408082000.jpg',
          );
          const t2 = new Date();
          console.log(t2 - t1);
          setAvg(preState => {
            return {
              ...preState,
              syncJsi: (preState.syncJsi + (t2 - t1)) / 2,
            };
          });
        }}
        style={styles.button}>
        <Text style={styles.buttonTxt}>sync Jsi</Text>
      </TouchableOpacity>
      <TouchableOpacity
        onPress={() => {
          const t1 = new Date();
          simpleJsiModule.foo(
            '/storage/emulated/0/Android/data/com.example/files/Pictures/IMG20220408082000.jpg',
            (error, result) => {
              const t2 = new Date();
              console.log(t2 - t1);
              // setAsyncJsi(result);
              setAvg(preState => {
                return {
                  ...preState,
                  asyncJsi: (preState.asyncJsi + (t2 - t1)) / 2,
                };
              });
            },
          );
        }}
        style={styles.button}>
        <Text style={styles.buttonTxt}>sync callback</Text>
      </TouchableOpacity>
      <TouchableOpacity
        onPress={() => {
          setAsyncJsi(null);
        }}
        style={styles.button}>
        <Text style={styles.buttonTxt}>reset</Text>
      </TouchableOpacity>
      {asyncJsi ? (
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

      <TouchableOpacity
        onPress={() => {
          const t1 = new Date();
          RNFetchBlob.fs
            .readFile(
              '/storage/emulated/0/Android/data/com.example/files/Pictures/IMG20220408082000.jpg',
              'base64',
            )
            .then(set => {
              const t2 = new Date();
              console.log(t2 - t1);
              // setrn(set);
              setAvg(preState => {
                return {
                  ...preState,
                  RNFetchBlob: (preState.RNFetchBlob + (t2 - t1)) / 2,
                };
              });
            });
        }}
        style={styles.button}>
        <Text style={styles.buttonTxt}>RN fetch blob</Text>
      </TouchableOpacity>
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
