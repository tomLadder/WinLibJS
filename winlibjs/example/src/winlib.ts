export enum Status {
  SUCESS = 0,
  FAILED,
  ACCESSDENIED,
  PEINVALID
}

type Callback = (status: Status) => any;

export interface Winlib {
  ManualMap(pid: number, path: string, callback: Callback): void
}

var winlib: Winlib = require('../../build/Release/winlib');

export default winlib;