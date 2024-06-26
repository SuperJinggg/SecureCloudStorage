package com.sklois.SecureCloudStorageSystem.controller;

import com.alibaba.fastjson.JSONObject;
import com.sklois.SecureCloudStorageSystem.entity.RegisterUser;
import com.sklois.SecureCloudStorageSystem.intercepter.Auth;
import com.sklois.SecureCloudStorageSystem.repository.RegisterUserRepository;
import com.sklois.SecureCloudStorageSystem.service.MetadataSer;
import jakarta.servlet.ServletException;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;
import jakarta.servlet.http.HttpServletRequest;

import java.io.IOException;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.List;

@RestController
public class DataListController {

    @Autowired
    private MetadataSer metadataser;
    @Autowired
    private RegisterUserRepository registeruserrepository;

    @RequestMapping("/folderstructurelist")
    @ResponseBody
    @Auth(normaluser = "普通用户", systemadmin = "系统管理员")
    public Object folderstructurelist(@RequestBody JSONObject json,HttpServletRequest request) throws IOException, ServletException {
        System.out.println("in folderstructurelist");
        if (null == json) {
            return null;
        }
        String sessionid = request.getHeader("sessionid");
        RegisterUser findout = registeruserrepository.findOneBySessionid(sessionid);
        List<JSONObject> folderlist= metadataser.folderlist(findout.getName());
        JSONObject ret = new JSONObject();
        ret.put("folderstructure",folderlist);
        return response("folderstructurelist", "success", "8000",
                "folderstructurelist success", ret);
    }

    @RequestMapping("/filelist")
    @ResponseBody
    @Auth(normaluser = "普通用户", systemadmin = "系统管理员")
    public Object filelist(@RequestBody JSONObject json, HttpServletRequest request) {
        if (null == json) {
            return null;
        }
        System.out.println("json is " + json.toJSONString());
        String method = json.getString("method");
        String version = json.getString("version");
        String timestamp = json.getString("timestamp");
        JSONObject requestjsonobj = json.getJSONObject("request");
        String folderuniqueid = requestjsonobj.getString("folderuniqueid");
        int pagenum = requestjsonobj.getIntValue("pagenum");
        int pagesize = requestjsonobj.getIntValue("pagesize");
        System.out.println("folderuniqueid is " + folderuniqueid);
        System.out.println("pagenum is " + pagenum);
        System.out.println("pagesize is " + pagesize);
        String sessionid = request.getHeader("sessionid");
        RegisterUser findout = registeruserrepository.findOneBySessionid(sessionid);
        List<JSONObject> filelist= metadataser.filelist(pagenum,pagesize,folderuniqueid,findout.getName());
        long filenumber = metadataser.countByParentidAndFileowner(folderuniqueid,findout.getName());
        JSONObject ret = new JSONObject();
        ret.put("filelist",filelist);
        ret.put("filenumber",filenumber);
        return response("filelist", "success", "8000",
                "filelist success", ret);

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
