package com.mcz.smoke_appproject.app.model;

import java.io.Serializable;

/**
 * Created by mcz on 2018/1/10.
 */

public class DataInfo implements Serializable{
    private String DeviceId ;
    private String DeviceName;
    private String DeviceStatus;
    private String DeviceType;
    private String error_code;
    private String lasttime;
    private String Devicetimestamp;
    ////////////////////////////////

    private String DeviceCSQ;
    private String DeviceSmokeValue;
    private String DeviceMaxValue;

    public DataInfo(){

    }
    public void setDeviceSmokeValue(String dv_SmokeValue) {
        DeviceSmokeValue = dv_SmokeValue;
    }

    public String getDeviceSmokeValue() {
        return DeviceSmokeValue;
    }


    public void setDeviceCSQ(String dv_CSQ) {
        DeviceCSQ = dv_CSQ;
    }

    public String getDeviceCSQ() {
        return DeviceCSQ;
    }

    public void setDeviceMaxValue(String dv_MaxValue) {
        DeviceMaxValue = dv_MaxValue;
    }

    public String getDeviceMaxValue() {
        return DeviceMaxValue;
    }

    public String getError_code() {
        return error_code;
    }

    public void setError_code(String error_code) {
        this.error_code = error_code;
    }



    public String getLasttime() {
        return lasttime;
    }

    public void setLasttime(String lasttime) {
        this.lasttime = lasttime;
    }

    public String getDeviceName() {
        return DeviceName;
    }

    public void setDeviceName(String deviceName) {
        DeviceName = deviceName;
    }


    public String getDeviceStatus() {
        return DeviceStatus;
    }

    public void setDevicetimestamp(String timestamp) {
        Devicetimestamp = timestamp;
    }



    public String getDevicetimestamp() {
        return Devicetimestamp;
    }

    public void setDeviceStatus(String deviceStatus) {
        DeviceStatus = deviceStatus;
    }

    public String getDeviceType() {
        return DeviceType;
    }

    public void setDeviceType(String deviceType) {
        DeviceType = deviceType;
    }

    public String getDeviceId() {
        return DeviceId;
    }

    public void setDeviceId(String deviceId) {
        this.DeviceId = deviceId;
    }
    public String getGatewayId() {
        return DeviceId;
    }

    public void setGatewayId(String deviceId) {
        this.DeviceId = deviceId;
    }
}
