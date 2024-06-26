package com.sklois.SecureCloudStorageSystem.controller;

import com.alibaba.fastjson.JSONObject;
import com.sklois.SecureCloudStorageSystem.entity.DataShareEntity;
import com.sklois.SecureCloudStorageSystem.entity.RegisterUser;
import com.sklois.SecureCloudStorageSystem.intercepter.Auth;
import com.sklois.SecureCloudStorageSystem.service.DataShareSer;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.bind.annotation.RestController;

import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import jakarta.servlet.http.HttpSession;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.HashMap;
import java.util.List;

@RestController
public class DataShareController {


    @Autowired
    private DataShareSer datashareser;

    @RequestMapping("/sharedata")
    @ResponseBody
    @Auth(normaluser = "普通用户", securityadmin = "系统管理员")
    public Object sharedata(
            @RequestBody JSONObject json,
            HttpServletRequest request, HttpServletResponse response,
            HttpSession session) {
        if (null == json) {
            return null;
        }
        String sessionid = request.getHeader("sessionid");
        String method = json.getString("method");
        String version = json.getString("version");
        String timestamp = json.getString("timestamp");
        JSONObject requestjsonobj = json.getJSONObject("request");
        String fileuniqueid = requestjsonobj.getString("fileuniqueid");
        String sharedusername = requestjsonobj.getString("sharedusername");
        HashMap<String, Object> savemessage = datashareser.sharedata(
                sessionid,
                fileuniqueid,
                sharedusername);
        boolean saveresult = (boolean) savemessage.get("result");
        String saveretstr = (String) savemessage.get("message");
        DataShareEntity saveret = (DataShareEntity) savemessage.get("saveret");
        if (saveresult) {
            JSONObject ret = new JSONObject();
            ret.put("fileuniqueid", fileuniqueid);
            ret.put("sharedusername", sharedusername);
            return response(method, "success", "8000",
                    "sharedata success", ret);
        } else {

            JSONObject ret = new JSONObject();
            return response(method, "fail", "8005",
                    saveretstr, ret);
        }
    }

    @RequestMapping("/getsharelist")
    @ResponseBody
    @Auth(normaluser = "普通用户", securityadmin = "系统管理员")
    public Object getsharelist(@RequestBody JSONObject json, HttpServletRequest request, HttpServletResponse response,
                               HttpSession session) {
        if (null == json) {
            return null;
        }
        String sessionid = request.getHeader("sessionid");
        String method = json.getString("method");
        String version = json.getString("version");
        String timestamp = json.getString("timestamp");

        HashMap<String, Object> getret = datashareser.getsharelist(sessionid);
        boolean getretresult = (boolean) getret.get("result");
        String getretmessage  = (String) getret.get("message");
        if(getretresult){
            int filenumber = (int) getret.get("filenumber");
            List<JSONObject> filelist = (List<JSONObject>) getret.get("filelist");
            JSONObject ret = new JSONObject();
            ret.put("filenumber",filenumber);
            ret.put("filelist",filelist);
            return response("getsharelist", "success", "8000",
                    getretmessage, ret);
        }else{
            JSONObject ret = new JSONObject();
            return response("getsharelist", "fail", "8005",
                    getretmessage, ret);
        }

    }

    @RequestMapping("/cancelshare")
    @ResponseBody
    @Auth(normaluser = "普通用户", securityadmin = "系统管理员")
    public Object cancelshare(@RequestBody JSONObject json, HttpServletRequest request, HttpServletResponse response,
                              HttpSession session) {
        if (null == json) {
            return null;
        }
        String sessionid = request.getHeader("sessionid");
        String method = json.getString("method");
        String version = json.getString("version");
        String timestamp = json.getString("timestamp");
        JSONObject requestjsonobj = json.getJSONObject("request");
        String fileuniqueid = requestjsonobj.getString("fileuniqueid");
        String sharedusername = requestjsonobj.getString("sharedusername");


        HashMap<String, Object> savemessage =
                datashareser.cancelshare(
                        sessionid, fileuniqueid, sharedusername);

        JSONObject ret = new JSONObject();
        ret.put("fileuniqueid", fileuniqueid);
        ret.put("sharedusername", sharedusername);
        return response("cancelshare", "success", "8000",
                "cancelshare success", ret);
    }


    @RequestMapping("/getsharedlist")
    @ResponseBody
    @Auth(normaluser = "普通用户", securityadmin = "系统管理员")
    public Object getsharedlist(@RequestBody JSONObject json, HttpServletRequest request, HttpServletResponse response,
                                HttpSession session) {

        if (null == json) {
            return null;
        }
        String sessionid = request.getHeader("sessionid");
        String method = json.getString("method");
        String version = json.getString("version");
        String timestamp = json.getString("timestamp");
        HashMap<String, Object> getret = datashareser.getsharedlist(sessionid);
        boolean getretresult = (boolean) getret.get("result");
        String getretmessage  = (String) getret.get("message");
        if(getretresult){
            int filenumber = (int) getret.get("filenumber");
            List<JSONObject> filelist = (List<JSONObject>) getret.get("filelist");
            JSONObject ret = new JSONObject();
            ret.put("filenumber",filenumber);
            ret.put("filelist",filelist);
            return response("getsharedlist", "success", "8000",
                    getretmessage, ret);
        }else{
            JSONObject ret = new JSONObject();
            return response("getsharedlist", "fail", "8005",
                    getretmessage, ret);
        }
    }


    private JSONObject response(String method, String result, String code,
                                String message, JSONObject details) {
        JSONObject ret = new JSONObject();
        ret.put("method", method);
        ret.put("result", result);
        ret.put("code", code);
        ret.put("message", message);
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
        LocalDateTime currentTime = LocalDateTime.now(); // get the current date and time
        String currenttimestr = currentTime.format(formatter);
        ret.put("timestamp", currenttimestr);
        ret.put("details", details);
        return ret;
    }
}
