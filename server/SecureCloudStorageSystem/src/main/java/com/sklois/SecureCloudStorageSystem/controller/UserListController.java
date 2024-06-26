package com.sklois.SecureCloudStorageSystem.controller;

import com.alibaba.fastjson.JSONObject;
import com.sklois.SecureCloudStorageSystem.entity.RegisterUser;
import com.sklois.SecureCloudStorageSystem.intercepter.Auth;
import com.sklois.SecureCloudStorageSystem.service.AuditSerImpl;
import com.sklois.SecureCloudStorageSystem.service.DatabaseSer;
import com.sklois.SecureCloudStorageSystem.service.UserListSer;
import com.sklois.SecureCloudStorageSystem.util.IpUtil;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import jakarta.servlet.http.HttpSession;
import org.springframework.beans.factory.annotation.Autowired;
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
public class UserListController {

    @Autowired
    DatabaseSer databaseser;
    @Autowired
    UserListSer userlistser;
    @Autowired
    AuditSerImpl auditserimpl;

    @RequestMapping("/userlist")
    @ResponseBody
    @Auth(securityadmin = "安全保密管理员")
    public Object userlist(@RequestBody JSONObject json, HttpServletRequest request, HttpServletResponse response, HttpSession session) throws InvalidKeyException, NoSuchAlgorithmException {
        if (null == json) {
            return null;
        }
        String sessionid = request.getHeader("sessionid");

        if (null == sessionid || sessionid.length() <= 0) {
            return response("auditloglist", "fail",
                    "8010", "sessionid missing",null);
        }
        HashMap<String, Object> findout = databaseser.verifysessionid(sessionid);
        String role="";
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
        role = requestobj.getString("role");
        String ipaddress = IpUtil.getIpAddr(request);

        List<RegisterUser> listout = userlistser.listByRole(role);
        long number = userlistser.countByRole(role);

        List<JSONObject> userItems = new ArrayList<>();
        for (int i = 0; i < listout.size(); i++) {
            RegisterUser data = listout.get(i);
            // 封装返回JSON数据
            JSONObject auditItem = new JSONObject();
            auditItem.put("id", data.getId());
            auditItem.put("registertime", data.getRegisterdate());
            auditItem.put("forbidtime", null);
            auditItem.put("username", data.getName());
            auditItem.put("role", data.getRole());
            auditItem.put("state", null);
            userItems.add(auditItem);
        }

        JSONObject jsonObject = new JSONObject();
        jsonObject.put("userentity_list", userItems);
        jsonObject.put("usernumber",number);

        LocalDateTime currentTime = LocalDateTime.now(); // get the current date and time
        auditserimpl.auditLogSave(
                currentTime,
                ipaddress,
                username,
                role,
                "用户查询",
                "用户查询模块",
                "获取成功",
                "success",
                "终止状态");

        return response(method,
                "success",
                "8000",
                "audit log list success",
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
