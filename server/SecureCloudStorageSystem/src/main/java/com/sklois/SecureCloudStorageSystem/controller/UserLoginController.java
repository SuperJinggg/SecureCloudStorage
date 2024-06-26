package com.sklois.SecureCloudStorageSystem.controller;

import com.alibaba.fastjson.JSONObject;
import com.sklois.SecureCloudStorageSystem.entity.RegisterUser;
import com.sklois.SecureCloudStorageSystem.service.AuditSerImpl;
import com.sklois.SecureCloudStorageSystem.service.DatabaseSer;
import com.sklois.SecureCloudStorageSystem.util.IpUtil;
import com.sklois.SecureCloudStorageSystem.util.LoginUtil;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import javax.imageio.ImageIO;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import jakarta.servlet.http.HttpSession;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.io.OutputStream;
import java.io.UnsupportedEncodingException;
import java.security.InvalidKeyException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.time.Duration;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.*;


@RestController
public class UserLoginController {
    @Autowired
    DatabaseSer databaseser;
    @Autowired
    AuditSerImpl auditserimpl;

    @RequestMapping("/userauthinit")
//    @Auth(user = "数据管理员", normaluser = "普通用户",useradmin="用户管理员")
    public void userauthinit(@RequestBody JSONObject json, HttpServletRequest request, HttpServletResponse response, HttpSession session) throws IOException {
        if (null == json) {
            return;
        }
        String method = json.getString("method");
        String version = json.getString("version");
        String timestamp = json.getString("timestamp");

        if (null == method || 0 != method.compareTo("userauthinit")) {
            return;
        }
        if (null == version || 0 != version.compareTo("1.0")) {
            return;
        }
        if (null == timestamp) {
            return;
        }
        LocalDateTime currentTime = LocalDateTime.now(); // get the current date and time
        String ipaddress = IpUtil.getIpAddr(request);
//        System.out.println("session id is " + session.getId());
        // 利用图片工具生成图片
        // 第一个参数是生成的验证码，第二个参数是生成的图片
        Object[] objs = LoginUtil.createImage();
        // 将验证码存入Session
        session.setAttribute("imageCode", objs[0]);

        String uuid = UUID.randomUUID().toString().replaceAll("-", "");
        session.setAttribute("random", uuid);
        // 将图片返回给客户端
        BufferedImage image = (BufferedImage) objs[1];
        response.setContentType("image/png");

        response.addHeader("randomcode", uuid);
        response.addHeader("method", method);
        OutputStream os = response.getOutputStream();


        auditserimpl.auditLogSave(
                currentTime,
                ipaddress,
                "",
                "",
                "用户鉴别初始化",
                "用户鉴别模块",
                "获取验证码成功,验证码是" + objs[0],
                "success",
                "终止状态");

        ImageIO.write(image, "png", os);
    }


    @RequestMapping("/userauth")
    @ResponseBody
    public Object userauth(@RequestBody JSONObject json, HttpServletRequest request, HttpServletResponse response, HttpSession session) throws InvalidKeyException, NoSuchAlgorithmException {
        if (null == json) {
            return null;
        }
        String method = json.getString("method");
        String version = json.getString("version");
        String timestamp = json.getString("timestamp");
        JSONObject requestobj = json.getJSONObject("request");
        String username = requestobj.getString("username");
        String role = requestobj.getString("role");
        String authcode = requestobj.getString("authcode");
        String mac = requestobj.getString("mac");


        System.out.println("method is " + method);
        System.out.println("version is " + version);
        System.out.println("timestamp is " + timestamp);
        System.out.println("username is " + username);
        System.out.println("role is " + role);
        System.out.println("authcode is " + authcode);
        System.out.println("mac is " + mac);
        System.out.println("session id is " + session.getId());
        String getvalicode = (String) session.getAttribute("imageCode");
        System.out.println("getvalicode is " + getvalicode);

        //检查参数是否齐全,格式是否正确

        //检查请求是否超时
//        DateTimeFormatter df = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");//设置日期格式
//        LocalDateTime requesttime = LocalDateTime.parse(timestamp);
//        LocalDateTime currentTime = LocalDateTime.now();

//        Date requesttime =
//        DateTimeFormatter df = new DateTimeFormatter("yyyy-MM-dd HH:mm:ss");//设置日期格式
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
        LocalDateTime currentTime = LocalDateTime.now(); // get the current date and time
        LocalDateTime requesttime = LocalDateTime.parse(timestamp, formatter);
        Duration duration = Duration.between(requesttime, currentTime);
        String ipaddress = IpUtil.getIpAddr(request);
        if (duration.toMillis() > 2000) {
            auditserimpl.auditLogSave(
                    currentTime,
                    ipaddress,
                    username,
                    role,
                    "用户鉴别",
                    "用户鉴别模块",
                    "用户鉴别失败,请求超时",
                    "fail",
                    "终止状态");

            return response(method, "fail", "8010", "request time out", null);
        }

        //检查图形验证码是否正确
        if (null == getvalicode || 0 != getvalicode.compareToIgnoreCase(authcode)) {

            auditserimpl.auditLogSave(
                    currentTime,
                    ipaddress,
                    username,
                    role,
                    "用户鉴别",
                    "用户鉴别模块",
                    "用户鉴别失败,图像验证码错误",
                    "fail",
                    "终止状态");

            return response(method, "fail", "8011", "authcode fail", null);
        }


        //检查用户是否存在

        //验证mac值是否正确
        RegisterUser registeruser = (RegisterUser) databaseser.findOneByName(username);
        if (null != registeruser) {
            if(0!=role.compareToIgnoreCase(registeruser.getRole())){
                auditserimpl.auditLogSave(
                        currentTime,
                        ipaddress,
                        username,
                        role,
                        "用户鉴别",
                        "用户鉴别模块",
                        "用户鉴别失败",
                        "角色错误",
                        "终止状态");
                return response(method, "fail", "8007", "role failed", null);
            }

            String random = (String) session.getAttribute("random");
            System.out.println("random is " + random);
            boolean verifyresult = databaseser.macverity(registeruser, username, authcode, random, timestamp, mac);
            System.out.println("verifyresult is " + verifyresult);
            if (verifyresult) {
                response.addHeader("method", method);
                JSONObject userauthresult = new JSONObject();
                userauthresult.put("role", role);
                userauthresult.put("username", username);
                String sessionkey = UUID.randomUUID().toString().replace("-","");
                userauthresult.put("sessionkey", sessionkey);
                userauthresult.put("lastauthtime", registeruser.getLastauthtime());
                userauthresult.put("lastauthaddress", registeruser.getLastauthipaddress());
                MessageDigest messageDigest;
                byte[] digest = new byte[0];
                try {
                    messageDigest = MessageDigest.getInstance("SHA-256");
                    messageDigest.update((currentTime.toString() + sessionkey).getBytes("UTF-8"));
                    digest = messageDigest.digest();
                } catch (NoSuchAlgorithmException e) {
                    System.out.println("encode password error");
                    e.printStackTrace();
                } catch (UnsupportedEncodingException e) {
                    System.out.println("encode password error");
                    e.printStackTrace();
                }
                String sessionid = bytesToHex(digest);
                userauthresult.put("sessionid", sessionid);

                RegisterUser saveresult = databaseser.savesessioninfo(registeruser, sessionid, sessionkey, currentTime,ipaddress);


                auditserimpl.auditLogSave(
                        currentTime,
                        ipaddress,
                        username,
                        role,
                        "用户鉴别",
                        "用户鉴别模块",
                        "用户鉴别成功",
                        "success",
                        "终止状态");
                return response(method, "success", "8000", "userauth success", userauthresult);
            } else {
                response.addHeader("method", method);

                auditserimpl.auditLogSave(
                        currentTime,
                        ipaddress,
                        username,
                        role,
                        "用户鉴别",
                        "用户鉴别模块",
                        "用户鉴别失败",
                        "fail",
                        "终止状态");

                return response(method, "fail", "8007", "userauth failed", null);
            }
        } else {
            JSONObject findresult = new JSONObject();
            findresult.put("ifexist", "false");
            response.addHeader("method", method);
            auditserimpl.auditLogSave(
                    currentTime,
                    ipaddress,
                    username,
                    role,
                    "用户鉴别",
                    "用户鉴别模块",
                    "用户鉴别失败,用户不存在",
                    "fail",
                    "终止状态");
            return response(method, "fail", "8006", "user not exist", findresult);
        }


        //多次登录失败锁定账户


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
    public static String bytesToHex(byte[] bytes) {
        StringBuilder buf = new StringBuilder(bytes.length * 2);
        for(byte b : bytes) { // 使用String的format方法进行转换
            buf.append(String.format("%02x", b & 0xff));
        }

        return buf.toString();
    }
}
