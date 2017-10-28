import winlib, { Status } from './winlib'

winlib.ManualMap(7564, "C:\\Dev\\dxlib\\dxlib\\x64\\Release\\dx11.dll", (status: Status) => {
  if(status == Status.SUCESS) {
    console.log('Image successfully mapped into target process');
  } else {
    console.log('Error - Status: ' + Status[status]);
  }
});