/* eslint-disable prettier/prettier */
//@ts-ignore we want to ignore everything
// else in global except what we need to access.
// Maybe there is a better way to do this.
import { NativeModules } from 'react-native';

// Installing JSI Bindings as done by
// https://github.com/mrousavy/react-native-mmkv

//@ts-ignore
const rawJsiModule: {
  readSync():string;
  writeSync():string;
  readAsync(
    callback: (error: string | undefined, value: string | undefined) => void
  ): void;
  writeAsync(
    callback: (error: string | undefined, value: string | undefined) => void
  ): void;
  writeConcurrent(
    callback: (error: string | undefined, value: string | undefined) => void
  ): void;
  Dir: object;
  //@ts-ignore
} = global;

export function isLoaded() {
  return typeof rawJsiModule.readAsync === 'function';
}

if (!isLoaded()) {
  rawJsiModule.Dir=NativeModules.raw?.Dir();
  const result = NativeModules.raw?.install();
  if (!result && !isLoaded())
    throw new Error('JSI bindings were not installed for: raw Module');

  if (!isLoaded()) {
    throw new Error('JSI bindings were not installed for: raw Module');
  }
}
export default rawJsiModule;
