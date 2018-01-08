import winlib, { Status } from './winlib'

winlib.ManualMap(8908, "C:\\Users\\Thomas\\Desktop\\test\\hello.dll", (status: Status) => {
  if(status == Status.SUCESS) {
    console.log('Image successfully mapped into target process');
  } else {
    console.log('Error - Status: ' + Status[status]);
  }
});