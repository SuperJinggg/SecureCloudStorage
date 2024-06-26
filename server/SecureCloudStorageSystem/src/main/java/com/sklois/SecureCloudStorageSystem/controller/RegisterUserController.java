package com.sklois.SecureCloudStorageSystem.controller;

import com.sklois.SecureCloudStorageSystem.entity.RegisterUser;
import com.sklois.SecureCloudStorageSystem.intercepter.Auth;
import com.sklois.SecureCloudStorageSystem.service.AuditSerImpl;
import com.sklois.SecureCloudStorageSystem.service.DatabaseSer;
import com.sklois.SecureCloudStorageSystem.util.IpUtil;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;
import java.text.ParseException;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.HashMap;
import java.util.List;
import com.alibaba.fastjson.JSONObject;
import jakarta.servlet.http.HttpServletRequest;

@RestController
public class RegisterUserController {

    @Autowired
    DatabaseSer databaseser;
    @Autowired
    AuditSerImpl auditserimpl;


    @RequestMapping("/checkusernameexistence")
    @ResponseBody
    public Object checkusernameexistence(@RequestBody JSONObject json,HttpServletRequest request) {
        if (null == json) {
            return null;
        }
        String method = json.getString("method");
        String version = json.getString("version");
        String timestamp = json.getString("timestamp");
        JSONObject requestobj = json.getJSONObject("request");
        String username = requestobj.getString("username");


        String ipaddress = IpUtil.getIpAddr(request);
        LocalDateTime currentTime = LocalDateTime.now(); // get the current date and time
        /**
         * 检查参数是否合格
         */
        if (null == method || 0 != method.compareTo("checkusernameexistence")) {
            auditserimpl.auditLogSave(
                    currentTime,
                    ipaddress,
                    username,
                    "",
                    "检查用户名是否存在",
                    "用户标识模块",
                    "请求参数method错误",
                    "fail",
                    "终止状态");

            return response(method, "failed", "8001", "method error", null);
        }
        if (null == version || 0 != version.compareTo("1.0")) {

            auditserimpl.auditLogSave(
                    currentTime,
                    ipaddress,
                    username,
                    "",
                    "检查用户名是否存在",
                    "用户标识模块",
                    "请求参数version错误",
                    "fail",
                    "终止状态");

            return response(method, "failed", "8002", "version error", null);
        }
        if (null == timestamp) {
            auditserimpl.auditLogSave(
                    currentTime,
                    ipaddress,
                    username,
                    "",
                    "检查用户名是否存在",
                    "用户标识模块",
                    "请求参数timestamp错误",
                    "fail",
                    "终止状态");

            return response(method, "failed", "8003", "timestamp error", null);
        }
        if (null == username) {
            auditserimpl.auditLogSave(
                    currentTime,
                    ipaddress,
                    username,
                    "",
                    "检查用户名是否存在",
                    "用户标识模块",
                    "请求参数username错误",
                    "fail",
                    "终止状态");

            return response(method, "failed", "8004", "username error", null);
        }

        System.out.println("method is " + method);
        System.out.println("version is " + version);
        System.out.println("timestamp is " + timestamp);
        System.out.println("username is " + username);

        HashMap<String, Object> findout = databaseser.findByName(username);

        boolean retresult = (boolean) findout.get("result");
        String retmessage = (String) findout.get("message");
        List<RegisterUser> findoutlist = (List<RegisterUser>) findout.get("findout");

        System.out.println("retresult is " + retresult);
        System.out.println("message is " + retmessage);

        if (retresult) {
            if (null != findoutlist && findoutlist.size() > 0) {
                JSONObject findresult = new JSONObject();
                findresult.put("ifexist", "true");
                auditserimpl.auditLogSave(
                        currentTime,
                        ipaddress,
                        username,
                        "",
                        "检查用户名是否存在",
                        "用户标识模块",
                        "用户名存在",
                        "success",
                        "终止状态");
                return response(method, "success", "8000", retmessage, findresult);
            } else {
                JSONObject findresult = new JSONObject();
                findresult.put("ifexist", "false");
                auditserimpl.auditLogSave(
                        currentTime,
                        ipaddress,
                        username,
                        "",
                        "检查用户名是否存在",
                        "用户标识模块",
                        "用户名不存在",
                        "success",
                        "终止状态");
                return response(method, "success", "8000", retmessage, findresult);
            }
        } else {
            JSONObject findresult = new JSONObject();
            findresult.put("ifexist", "false");

            auditserimpl.auditLogSave(
                    currentTime,
                    ipaddress,
                    username,
                    "",
                    "检查用户名是否存在",
                    "用户标识模块",
                    "用户名不存在",
                    "success",
                    "终止状态");

            return response(method, "success", "8000", retmessage, findresult);
        }
    }

    @RequestMapping("/userregister")
    @ResponseBody
    public Object userregister(@RequestBody JSONObject json, HttpServletRequest servletrequest) throws ParseException {
        if (null == json) {
            return null;
        }
        System.out.println("json is " + json.toJSONString());
        String method = json.getString("method");
        String version = json.getString("version");
        String timestamp = json.getString("timestamp");
        JSONObject request = json.getJSONObject("request");
        String username = request.getString("username");
        String password = request.getString("password");
        String role = request.getString("role");

        System.out.println("method is " + method);
        System.out.println("version is " + version);
        System.out.println("timestamp is " + timestamp);
        System.out.println("username is " + username);
        System.out.println("password is " + password);
        System.out.println("role is " + role);
        String ipaddress = IpUtil.getIpAddr(servletrequest);
        LocalDateTime currentTime = LocalDateTime.now(); // get the current date and time
        /**
         * 检查参数是否合格
         */
        if (null == method || 0 != method.compareTo("userregister")) {
            auditserimpl.auditLogSave(
                    currentTime,
                    ipaddress,
                    username,
                    "",
                    "用户标识",
                    "用户标识模块",
                    "请求参数method错误",
                    "fail",
                    "终止状态");
            return response(method, "failed", "8001", "method error", null);
        }
        if (null == version || 0 != version.compareTo("1.0")) {
            auditserimpl.auditLogSave(
                    currentTime,
                    ipaddress,
                    username,
                    "",
                    "用户标识",
                    "用户标识模块",
                    "请求参数version错误",
                    "fail",
                    "终止状态");
            return response(method, "failed", "8002", "version error", null);
        }
        if(null==timestamp||timestamp.length()<=0){
            auditserimpl.auditLogSave(
                    currentTime,
                    ipaddress,
                    username,
                    "",
                    "用户标识",
                    "用户标识模块",
                    "请求参数timestamp错误",
                    "fail",
                    "终止状态");
            return response(method, "failed", "8003", "timestamp error", null);

        }
        if(null==username||username.length()<=0){
            auditserimpl.auditLogSave(
                    currentTime,
                    ipaddress,
                    username,
                    "",
                    "用户标识",
                    "用户标识模块",
                    "请求参数username错误",
                    "fail",
                    "终止状态");
            return response(method, "failed", "8004", "username error", null);

        }if(null==password||password.length()<=0){
            auditserimpl.auditLogSave(
                    currentTime,
                    ipaddress,
                    username,
                    "",
                    "用户标识",
                    "用户标识模块",
                    "请求参数password错误",
                    "fail",
                    "终止状态");
            return response(method, "failed", "8005", "password error", null);

        }if(null==role||role.length()<=0){

        }

//        SimpleDateFormat simpleDateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
//        String dateStr = "2019-01-03 10:59:27";
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
        LocalDateTime localdatetime = LocalDateTime.parse(timestamp,formatter);
        System.out.println("date is " + localdatetime.toString());
        HashMap<String, Object> saveresult = databaseser.save(username,password,role,localdatetime);

        boolean retresult = (boolean) saveresult.get("result");
        String retmessage = (String) saveresult.get("message");
        System.out.println("retresult is " + retresult);
        System.out.println("message is " + retmessage);
        if(retresult){
            String saveusername = (String) saveresult.get("username");
            String saverole = (String) saveresult.get("role");

            JSONObject retresultjson = new JSONObject();
            retresultjson.put("username",saveusername);
            retresultjson.put("role",saverole);


            auditserimpl.auditLogSave(
                    currentTime,
                    ipaddress,
                    username,
                    "",
                    "用户标识",
                    "用户标识模块",
                    "用户标识成功",
                    "success",
                    "终止状态");


            return response(method, "success", "8000", retmessage, retresultjson);
        }else{
            if(0==retmessage.compareTo("save error")){
                JSONObject retresultjson = new JSONObject();
                retresultjson.put("username", null);


                auditserimpl.auditLogSave(
                        currentTime,
                        ipaddress,
                        username,
                        "",
                        "用户标识",
                        "用户标识模块",
                        "用户标识信息存储失败",
                        "fail",
                        "终止状态");

                return response(method, "fail", "8005", retmessage, retresultjson);
            }else if(0==retmessage.compareTo("username exist")){
                JSONObject retresultjson = new JSONObject();
                auditserimpl.auditLogSave(
                        currentTime,
                        ipaddress,
                        username,
                        "",
                        "用户标识",
                        "用户标识模块",
                        "用户名已存在",
                        "fail",
                        "终止状态");
                return response(method, "fail", "8006", retmessage, retresultjson);
            }else{
                JSONObject retresultjson = new JSONObject();
                auditserimpl.auditLogSave(
                        currentTime,
                        ipaddress,
                        username,
                        "",
                        "用户标识",
                        "用户标识模块",
                        "用户标识信息存储失败",
                        "fail",
                        "终止状态");
                return response(method, "fail", "8007", retmessage, retresultjson);
            }
        }
    }


    @RequestMapping("/userlist1")
    @ResponseBody
    public Object userlist(@RequestBody JSONObject json, HttpServletRequest servletrequest) throws ParseException {
        if (null == json) {
            return null;
        }

        HashMap<String, Object> findmap =databaseser.findAll();
        boolean retresult = (boolean) findmap.get("result");
        String retmessage = (String) findmap.get("message");
        List<RegisterUser> findout = (List<RegisterUser>)findmap.get("findout");
        if(retresult&&null!=findout){
            JSONObject retresultjson = new JSONObject();
            retresultjson.put("findout",findout);
            return response("userlist", "success", "8000", retmessage, retresultjson);
        }
        return null;
    }

    @RequestMapping("/userfind")
    @ResponseBody
    public Object userfind(@RequestBody JSONObject json, HttpServletRequest servletrequest) throws ParseException {
        if (null == json) {
            return null;
        }
        //用户鉴别
        //权限鉴别

        System.out.println("json is " + json.toJSONString());
        String method = json.getString("method");
        String version = json.getString("version");
        String timestamp = json.getString("timestamp");
        JSONObject request = json.getJSONObject("request");
        String username = request.getString("username");
        String password = request.getString("password");
        String role = request.getString("role");





        return null;
    }


    @RequestMapping("/usermodify")
    @ResponseBody
    @Auth(securityadmin = "安全保密管理员")
    public Object usermodify(@RequestBody JSONObject json, HttpServletRequest servletrequest) throws ParseException {
        if (null == json) {
            return null;
        }

        String method = json.getString("method");
        String version = json.getString("version");
        String timestamp = json.getString("timestamp");
        JSONObject request = json.getJSONObject("request");
        String username = request.getString("username");
        JSONObject userinfo = request.getJSONObject("userinfo");
        String role = userinfo.getString("role");
        RegisterUser ret =(RegisterUser) databaseser.modifyuser(username,role);

        JSONObject retjsonobj = new JSONObject();
        retjsonobj.put("id",ret.getId());
        retjsonobj.put("registertime",ret.getRegisterdate());
        retjsonobj.put("forbidtime",null);
        retjsonobj.put("username",ret.getName());
        retjsonobj.put("role",ret.getRole());
        retjsonobj.put("state",null);

        JSONObject retresultjson = new JSONObject();
        retresultjson.put("userinfo",retjsonobj);
        return response(
                "usermodify", "success",
                "8000", "user modify success", retresultjson);
    }


    @RequestMapping("/userdelete")
    @ResponseBody
    @Auth(securityadmin = "安全保密管理员")
    public Object userdelete(@RequestBody JSONObject json, HttpServletRequest servletrequest) throws ParseException {
        if (null == json) {
            return null;
        }

        String method = json.getString("method");
        String version = json.getString("version");
        String timestamp = json.getString("timestamp");
        JSONObject request = json.getJSONObject("request");
        String username = request.getString("username");
        RegisterUser ret =(RegisterUser) databaseser.deleteuser(username);
        if(ret!=null) {
            JSONObject retjsonobj = new JSONObject();
            retjsonobj.put("id", ret.getId());
            retjsonobj.put("registertime", ret.getRegisterdate());
            retjsonobj.put("forbidtime", null);
            retjsonobj.put("username", ret.getName());
            retjsonobj.put("role", ret.getRole());
            retjsonobj.put("state", null);

            JSONObject retresultjson = new JSONObject();
            retresultjson.put("userinfo", retjsonobj);
            return response(
                    "userdelete", "success",
                    "8000", "user delete success", retresultjson);
        }else
        {
            return response(
                    "userdelete", "fail",
                    "8001", "user delete failed", null);
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
