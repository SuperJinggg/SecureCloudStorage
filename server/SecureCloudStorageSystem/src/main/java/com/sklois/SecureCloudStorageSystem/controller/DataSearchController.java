package com.sklois.SecureCloudStorageSystem.controller;

import com.alibaba.fastjson.JSONArray;
import com.alibaba.fastjson.JSONObject;
import com.sklois.SecureCloudStorageSystem.entity.Metadata;
import com.sklois.SecureCloudStorageSystem.intercepter.Auth;
import com.sklois.SecureCloudStorageSystem.service.MetadataSerImpl;
import jakarta.servlet.http.HttpServletResponse;
import jakarta.servlet.http.HttpSession;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.bind.annotation.RestController;

import jakarta.servlet.http.HttpServletRequest;


import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.List;

@RestController
public class DataSearchController {

    @Autowired
    private MetadataSerImpl metadataser;
    @RequestMapping("/datasearch")
    @ResponseBody
    @Auth(normaluser = "普通用户", securityadmin = "系统管理员")
    public Object datasearch(@RequestBody JSONObject json, HttpServletRequest request, HttpServletResponse response,
                             HttpSession session) {
        if (json == null) {
            return null;
        }

        System.out.println("json is " + json.toJSONString());

        String sessionid = request.getHeader("sessionid");
        String method = json.getString("method");
        String version = json.getString("version");
        String timestamp = json.getString("timestamp");

        JSONObject requestjsonobj = json.getJSONObject("request");
        String mode = requestjsonobj.getString("mode");
        String condition = requestjsonobj.getString("condition");
        JSONArray keywords = requestjsonobj.getJSONArray("keywords");
        String type = requestjsonobj.getString("type");
        JSONObject timeregion = requestjsonobj.getJSONObject("timeregion");
        String fromtime = "";
        String totime = "";

        if (timeregion != null) {
            fromtime = timeregion.getString("fromtime");
            totime = timeregion.getString("totime");
        }

        int pagenum = requestjsonobj.getIntValue("pagenum");
        int pagesize = requestjsonobj.getIntValue("pagesize");

        List<Metadata> filelist = metadataser.datasearch(
                sessionid,
                mode,
                condition,
                keywords,
                type,
                fromtime,
                totime,
                pagenum,
                pagesize);

        JSONObject ret = new JSONObject();

        if (filelist != null) {
            ret.put("filelist", filelist);
            ret.put("filenumber", filelist.size());
        } else {
            ret.put("filelist", new ArrayList<>());
            ret.put("filenumber", 0);
        }

        return response("datasearch", "success", "8000", "datasearch success", ret);
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
