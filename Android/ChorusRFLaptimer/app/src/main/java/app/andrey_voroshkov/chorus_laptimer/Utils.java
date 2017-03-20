package app.andrey_voroshkov.chorus_laptimer;

/**
 * Created by Andrey_Voroshkov on 1/29/2017.
 */
public class Utils {
    private static Utils ourInstance = new Utils();

    public static Utils getInstance() {
        return ourInstance;
    }

    private Utils() {
    }

    public static String convertMsToHMS(int ms) {
        int h = (int)Math.floor(ms/1000/60/60);
        int m = (int)Math.floor(ms/1000/60)-h*60;
        int s = (int)Math.floor(ms/1000)-(h*60+m)*60;
        int msec = ms-(int)Math.floor(ms/1000)*1000;
        return String.format("%d : %02d : %02d . %03d", h,m,s, msec);
    }

    public static String btDataChunkParser(String chunk) {
        String result = "";

        if (chunk.length() < 2) {
            return result;
        }
        char dest = chunk.charAt(0);
        char module = chunk.charAt(1);
        int moduleId = -1;
        if (module != '*') {
            moduleId = Integer.parseInt(String.valueOf(module), 16);
        }
        if (dest == 'S') {
            result += "Module: " + ((moduleId >= 0) ? moduleId : "ALL") + "; ";
            char dataType = chunk.charAt(2);
            switch (dataType) {
                case 'C':
                    int channel = Integer.parseInt(chunk.substring(3,4), 16);
                    result += "Channel: " + channel;
                    AppState.getInstance().changeDeviceChannel(moduleId, channel);
                    break;
                case 'R':
                    int race = Integer.parseInt(chunk.substring(3,4));
                    result += "Race: " + ((race != 0) ? "started" : "stopped");
                    AppState.getInstance().changeRaceState(race!=0);
                    break;
                case 'M':
                    int minLapTime = Integer.parseInt(chunk.substring(3,5), 16);
                    result += "Min Lap: " + minLapTime;
                    AppState.getInstance().changeRaceMinLapTime(minLapTime);
                    break;
                case 'T':
                    int threshold = Integer.parseInt(chunk.substring(3,7), 16);
                    result += "Threshold: " + threshold;
                    AppState.getInstance().changeDeviceThreshold(moduleId, threshold);
                    break;
                case 'S':
                    int rssi = Integer.parseInt(chunk.substring(3,7), 16);
                    result += "RSSI: " + rssi;
                    AppState.getInstance().changeDeviceRSSI(moduleId, rssi);
                    break;
                case 'D':
                    int soundState = Integer.parseInt(chunk.substring(3,4));
                    result += "Sound: " + ((soundState != 0) ? "enabled" : "disabled");
                    AppState.getInstance().changeDeviceSoundState(soundState!=0);
                    break;
                case 'F':
                    int shouldSkipFirstLap = Integer.parseInt(chunk.substring(3,4));
                    result += "Skip First Lap: " + ((shouldSkipFirstLap != 0) ? "enabled" : "disabled");
                    AppState.getInstance().changeSkipFirstLap(shouldSkipFirstLap!=0);
                    break;
                case 'L':
                    int lapNo = Integer.parseInt(chunk.substring(3,5), 16);
                    int lapTime = Integer.parseInt(chunk.substring(5,13), 16);
                    result += "Lap #" + lapNo + " : " + lapTime;
                    AppState.getInstance().addLapResult(moduleId, lapNo, lapTime);
                    break;
                case 'B':
                    int band = Integer.parseInt(chunk.substring(3,4), 16);
                    result += "Band #" + band;
                    AppState.getInstance().changeDeviceBand(moduleId, band);
                    break;
                case 'i':
                    int isCalibrated = Integer.parseInt(chunk.substring(3,4), 16);
                    result += "Calibration: " + ((isCalibrated!= 0) ? "done" : "not performed");
                    AppState.getInstance().changeCalibration(moduleId, (isCalibrated!=0));
                    break;
                case 'I':
                    int calibrationTime = Integer.parseInt(chunk.substring(3,11), 16);
                    result += "Calibration time: " + calibrationTime;
                    AppState.getInstance().changeDeviceCalibrationTime(moduleId, calibrationTime);
                    break;
                case 'V':
                    int isMonitorOn = Integer.parseInt(chunk.substring(3,4), 16);
                    result += "RSSI Monitor: " + ((isMonitorOn!= 0) ? "on" : "off");
                    AppState.getInstance().changeRssiMonitorState(isMonitorOn!=0);
                    break;
                case 'X':
                    result += "EndOfSequence. Device# " + moduleId;
                    AppState.getInstance().receivedEndOfSequence(moduleId);
                    break;
            }
        } else if (dest == 'R') {

        } else if (dest == 'N') {
            result += "ModulesCount: " + module;
            AppState.getInstance().setNumberOfDevices(moduleId);
        }
        return result;
    }
}