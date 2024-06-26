package com.sklois.SecureCloudStorageSystem.controller;

import com.alibaba.fastjson.JSONObject;
import com.sklois.SecureCloudStorageSystem.entity.AuditEntity;
import com.sklois.SecureCloudStorageSystem.entity.RegisterUser;
import com.sklois.SecureCloudStorageSystem.intercepter.Auth;
import com.sklois.SecureCloudStorageSystem.service.AuditSer;
import com.sklois.SecureCloudStorageSystem.service.AuditSerImpl;
import com.sklois.SecureCloudStorageSystem.service.DatabaseSer;
import com.sklois.SecureCloudStorageSystem.util.IpUtil;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import jakarta.servlet.http.HttpSession;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.domain.Page;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.bind.annotation.RestController;


import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

@RestController
public class AuditLogController {
    @Autowired
    DatabaseSer databaseser;
    @Autowired
    AuditSer auditser;
    @Autowired
    AuditSerImpl auditserimpl;

    @RequestMapping("/auditloglist")
    @ResponseBody
    @Auth(securityadmin = "安全保密管理员", normaluser = "安全审计员")
    public Object auditloglist(@RequestBody JSONObject json, HttpServletRequest request, HttpServletResponse response, HttpSession session) throws InvalidKeyException, NoSuchAlgorithmException {
        if (null == json) {
            return null;
        }
        String sessionid = request.getHeader("sessionid");

        if (null == sessionid || sessionid.length() <= 0) {
            return response("auditloglist", "fail",
                    "8010", "sessionid missing",null);
        }
        HashMap<String, Object> findout = databaseser.verifysessionid(sessionid);
        String role = "";
        String username = "";
        RegisterUser registeruser;
        boolean result = (boolean) findout.get("result");
        System.out.println("result is " + result);
        if (result) {
            //session正确,获取角色
            registeruser = (RegisterUser) findout.get("entity");
            role = registeruser.getRole();
            username = registeruser.getName();
        } else {
            //session不正确,返回错误
            return response("auditloglist", "fail",
                    "8010", "sessionkey error",null);
        }

        String method = json.getString("method");
        String version = json.getString("version");
        String timestamp = json.getString("timestamp");
        JSONObject requestobj = json.getJSONObject("request");
        String auditlogrole = requestobj.getString("auditlogrole");
        String ipaddress = IpUtil.getIpAddr(request);

        if(0=="安全保密管理员".compareToIgnoreCase(role)){

            if(0=="普通用户".compareToIgnoreCase(auditlogrole)){

            }else if(0=="安全审计员".compareToIgnoreCase(auditlogrole)){

            }
        }else if(0=="安全审计员".compareToIgnoreCase(role)){
            if(0=="系统管理员".compareToIgnoreCase(auditlogrole)){

            }else if(0=="安全保密管理员".compareToIgnoreCase(auditlogrole)){

            }
        }

        AuditEntity auditentity  = new AuditEntity();
        auditentity.setRole(auditlogrole);
//        auditentity.setMethod("用户");
        Page<AuditEntity>  findpage = auditser.listByPage(0,20,auditentity);
        long number = auditser.countByRole(auditlogrole);
        List<AuditEntity> listout = findpage.getContent();
        System.out.println("查询结果个数 is "+listout.size());

        List<JSONObject> auditItems = new ArrayList<>();
        for (int i = 0; i < listout.size(); i++) {
            AuditEntity data = listout.get(i);
            // 封装返回JSON数据
            JSONObject auditItem = new JSONObject();
            auditItem.put("id", data.getId());
            auditItem.put("time", data.getTime());
            auditItem.put("ipaddress", data.getIpaddress());
            auditItem.put("username", data.getUsername());
            auditItem.put("role", data.getRole());
            auditItem.put("method", data.getMethod());
            auditItem.put("module", data.getModule());
            auditItem.put("object", data.getObject());
            auditItem.put("result", data.getResult());
            auditItem.put("status", data.getStatus());
            auditItems.add(auditItem);
        }

        JSONObject jsonObject = new JSONObject();
        jsonObject.put("auditlogrole",auditlogrole);
        jsonObject.put("auditlog",auditItems);
        jsonObject.put("auditlognumber",number);

        LocalDateTime currentTime = LocalDateTime.now(); // get the current date and time
        auditserimpl.auditLogSave(
                currentTime,
                ipaddress,
                username,
                role,
                "安全审计",
                "安全审计模块",
                "获取审计日志成功",
                "success",
                "终止状态");

        return response(method,
                "success",
                "8000",
                "audit log list success",
                jsonObject);

    }

    @RequestMapping("/auditlogsearch")
    @ResponseBody
    @Auth(securityadmin = "安全保密管理员", normaluser = "安全审计员")
    public Object auditlogsearch(@RequestBody JSONObject json, HttpServletRequest request, HttpServletResponse response, HttpSession session) throws InvalidKeyException, NoSuchAlgorithmException {
        if (null == json) {
            return null;
        }

        String sessionid = request.getHeader("sessionid");

        if (null == sessionid || sessionid.length() <= 0) {
            return response("auditloglist", "fail",
                    "8010", "sessionid missing",null);
        }
        HashMap<String, Object> findout = databaseser.verifysessionid(sessionid);
        String role = "";
        String username = "";
        RegisterUser registeruser;
        boolean result = (boolean) findout.get("result");
        System.out.println("result is " + result);
        if (result) {
            //session正确,获取角色
            registeruser = (RegisterUser) findout.get("entity");
            role = registeruser.getRole();
            username = registeruser.getName();
        } else {
            //session不正确,返回错误
            return response("auditloglist", "fail",
                    "8010", "sessionkey error",null);
        }

        String method = json.getString("method");
        String version = json.getString("version");
        String timestamp = json.getString("timestamp");
        JSONObject requestobj = json.getJSONObject("request");
        String auditlogrole = requestobj.getString("auditlogrole");
        String fromtime = requestobj.getString("fromtime");
        String totime = requestobj.getString("totime");
        String user = requestobj.getString("user");
        String module = requestobj.getString("module");
        int pagenum = requestobj.getIntValue("pagenum");
        int pagesize = requestobj.getIntValue("pagesize");
        String ipaddress = IpUtil.getIpAddr(request);

        AuditEntity auditentity  = new AuditEntity();
        auditentity.setRole(auditlogrole);
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
        LocalDateTime starttime = LocalDateTime.parse(fromtime,formatter);
        LocalDateTime endtime = LocalDateTime.parse(totime,formatter);

//        module = null;
//        user=null;

        Page<AuditEntity>  findpage = auditser.auditLogSearch(starttime,endtime,user,module,pagenum,pagesize,auditlogrole);
        long number = auditser.countAuditLog(starttime,endtime,user,module,auditlogrole);
        List<AuditEntity> listout = findpage.getContent();
        System.out.println("查询结果个数 is "+listout.size());

        List<JSONObject> auditItems = new ArrayList<>();
        for (int i = 0; i < listout.size(); i++) {
            AuditEntity data = listout.get(i);
            // 封装返回JSON数据
            JSONObject auditItem = new JSONObject();
            auditItem.put("id", data.getId());
            auditItem.put("time", data.getTime());
            auditItem.put("ipaddress", data.getIpaddress());
            auditItem.put("username", data.getUsername());
            auditItem.put("role", data.getRole());
            auditItem.put("method", data.getMethod());
            auditItem.put("module", data.getModule());
            auditItem.put("object", data.getObject());
            auditItem.put("result", data.getResult());
            auditItem.put("status", data.getStatus());
            auditItems.add(auditItem);
        }

        JSONObject jsonObject = new JSONObject();
        jsonObject.put("auditlogrole",auditlogrole);
        jsonObject.put("auditlog",auditItems);
        jsonObject.put("auditlognumber",number);

        LocalDateTime currentTime = LocalDateTime.now(); // get the current date and time
        auditserimpl.auditLogSave(
                currentTime,
                ipaddress,
                username,
                role,
                "安全审计",
                "安全审计模块",
                "审计日志查询成功",
                "success",
                "终止状态");

        return response(method,
                "success",
                "8000",
                "audit log search success",
                jsonObject);
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
