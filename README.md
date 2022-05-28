# react-native-raw

This is a React Native JSI read and Write library that works 2x faster then the conventional React Native read and Write library .

![Screenshot-_56_](https://user-images.githubusercontent.com/58332892/170806978-1d0e02d9-aae3-4728-a3fd-a3b990683a98.svg)
![imageCrop-min](https://user-images.githubusercontent.com/58332892/170808775-8bf12948-9826-4497-9dfc-5bc2bba7e7c2.png)


## Installation

```sh
npm install react-native-simple-jsi
```

AndroidManifest.xml

```xml
    <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" />
```

If you need to store visible use 
```xml
<application
...
android:requestLegacyExternalStorage="true"/>
```

For iOS also run `pod install` in `/ios` folder.


## Prerequisites
You must have Android NDK and CMake installed on android to build the library.

## Methods
The following methods are implemented.

### Directory 
Following default directory can be used.

```ts
import rawJsiModule from 'react-native-raw';

const {
DocumentDir,
CacheDir,
DCIMDir,
PictureDir,
MusicDir,
DownloadDir,
MovieDir,
RingtoneDir } = rawJsiModule.Dir

```

### RAW synchronous Write 

```ts
    // Data must be base64 
    writeSync(data:string,path:string):string;
```
### RAW synchronous Read 

```ts
    readSync(path:string):string;
```

### RAW asynchronous Write 

```ts
    // Data must be base64 
    readAsync(
    data:string, 
    path:string,
    callback(error:string,payload:string):void):string;
```
### RAW asynchronous Read 

```ts
     writeSync(
    path:string,
    callback(error:string,payload:string):void):string;
```


## Thanks to the template & author:
The initial work done by ammarahm-ed has helped a lot in writing this repo.

- [react-native-jsi-template](https://github.com/ammarahm-ed/react-native-jsi-template/)


## Contributing
See the [contributing guide](CONTRIBUTING.md) to learn how to contribute to the repository and the development workflow.

## License

MIT
