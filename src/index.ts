//@ts-ignore we want to ignore everything
// else in global except what we need to access.
// Maybe there is a better way to do this.
import { NativeModules } from 'react-native';

// Installing JSI Bindings as done by
// https://github.com/mrousavy/react-native-mmkv

//@ts-ignore
const simpleJsiModule: {
  foo(
    callback: (error: string | undefined, value: string | undefined) => void
  ): void;
  Dir: object;
  //@ts-ignore
} = global;

export function isLoaded() {
  return typeof simpleJsiModule.foo === 'function';
}

if (!isLoaded()) {
  NativeModules.raw?.Dir((event: any) => {
    simpleJsiModule.Dir = event;
  });
  const result = NativeModules.raw?.install();
  if (!result && !isLoaded())
    throw new Error('JSI bindings were not installed for: raw Module');

  if (!isLoaded()) {
    throw new Error('JSI bindings were not installed for: raw Module');
  }
}
export default simpleJsiModule;
