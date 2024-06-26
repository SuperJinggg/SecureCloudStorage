package com.sklois.SecureCloudStorageSystem.controller;

import com.alibaba.fastjson.JSONObject;
import com.sklois.SecureCloudStorageSystem.intercepter.Auth;
import com.sklois.SecureCloudStorageSystem.service.DockingSecurityCloudSer;
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


@RestController
public class GetCipherKeyController {


    @Autowired
    DockingSecurityCloudSer dockingsecuritycloudser;

    @RequestMapping("/getkey")
    @ResponseBody
    @Auth(normaluser = "普通用户")
    public Object getkey(@RequestBody JSONObject json, HttpServletRequest request, HttpServletResponse response,
                         HttpSession session) {
        if (null == json) {
            return null;
        }
        String sessionid  = request.getHeader("sessionid");
        System.out.println("json is " + json.toJSONString());
        String method = json.getString("method");
        String version = json.getString("version");
        String timestamp = json.getString("timestamp");
        JSONObject requestjsonobj = json.getJSONObject("request");
        String fileuniqueid = requestjsonobj.getString("fileuniqueid");
        String filekey = dockingsecuritycloudser.getkey(sessionid, fileuniqueid);
        JSONObject ret = new JSONObject();
        ret.put("fileuniqueid",fileuniqueid);
        ret.put("fileencryptkey",filekey);
        return response("getkey", "success", "8000",
                "getkey success", ret);
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
