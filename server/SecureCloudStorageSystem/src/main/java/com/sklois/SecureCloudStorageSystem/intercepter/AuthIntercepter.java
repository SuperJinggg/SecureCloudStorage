package com.sklois.SecureCloudStorageSystem.intercepter;

import com.alibaba.fastjson.JSONException;
import com.alibaba.fastjson.JSONObject;
import com.sklois.SecureCloudStorageSystem.entity.RegisterUser;
import com.sklois.SecureCloudStorageSystem.service.DatabaseSer;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import org.springframework.web.method.HandlerMethod;
import org.springframework.web.servlet.HandlerInterceptor;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.HashMap;


@Component
public class AuthIntercepter implements HandlerInterceptor {

    @Autowired
    DatabaseSer databaseser;

    @Override
    public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler)
            throws Exception {
        System.out.println("in preHandle");
        String sessionid = request.getHeader("sessionid");

        if (null == sessionid || sessionid.length() <= 0) {
            return false;
        }
        System.out.println("sessionid is " + sessionid);

        HashMap<String, Object> findout = databaseser.verifysessionid(sessionid);
        String role = "";
        RegisterUser registeruser;
        boolean result = (boolean) findout.get("result");
        System.out.println("result is " + result);
        if (result) {
            //session正确,获取角色
            registeruser = (RegisterUser) findout.get("entity");
            role = registeruser.getRole();
        } else {
            //session不正确,返回错误
            JSONObject res = new JSONObject();
            res.put("success", "false");
            res.put("msg", "xxxx");
            response(response,res);
            return false;
        }

        Auth auth = null;
        if (handler instanceof HandlerMethod) {
            auth = ((HandlerMethod) handler).getMethod().getAnnotation(Auth.class);
        } else {
            return false;
        }
        if (auth == null) {
            System.out.println("can't find @Auth in this uri:" + request.getRequestURI());
            return false;
        }
        // // 从参数中取出用户身份并验证
        String normaluser = auth.normaluser();
        String systemadmin = auth.systemadmin();
        String securityadmin = auth.securityadmin();
        String auditadmin = auth.auditadmin();
        if ((null != normaluser && normaluser.equals(role)) ||
                (null != systemadmin && systemadmin.equals(role)) ||
                (null != securityadmin && securityadmin.equals(role)) ||
                (null != auditadmin && auditadmin.equals(role))
        ) {
            System.out.println("当前用户已登录，登录的用户名为： " + registeruser.getName());
            return true;
        }

        return false;
    }

    public void response(HttpServletResponse response ,JSONObject retobject) throws JSONException, IOException {
        response.setCharacterEncoding("UTF-8");
        response.setContentType("application/json; charset=utf-8");
        response.setHeader("method", "userauth");
        PrintWriter out = null;
//        res.put("method", "userauth");
        out = response.getWriter();
        out.print(retobject.toString());
        out.flush();
        out.close();
//        BufferedReader reader = new BufferedReader(new InputStreamReader(request.getInputStream()));
//        String line = null;
//        StringBuilder sb = new StringBuilder();
//        while((line = reader.readLine())!=null){
//            sb.append(line);
//        }
//        String postContent = sb.toString();
//        System.out.println("postContent");
    }


}
