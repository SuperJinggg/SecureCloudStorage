package com.sklois.SecureCloudStorageSystem.controller;

import com.alibaba.fastjson.JSONObject;
import jakarta.servlet.ServletException;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.bind.annotation.RestController;

import jakarta.servlet.http.HttpServletRequest;
import java.io.IOException;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

@RestController
public class CheckUpdateController {

    @Value("${sklois.client.version}")
    private float currentversion;

    @RequestMapping("/getcurrentversion")
    @ResponseBody
    public Object getcurrentversion(@RequestBody JSONObject json, HttpServletRequest request) throws IOException, ServletException {
        System.out.println("in getcurrentversion");
        if (null == json) {
            return null;
        }
        JSONObject ret = new JSONObject();
        ret.put("currentversion",currentversion);
        return response("getcurrentversion", "success", "8000",
                "getcurrentversion success", ret);
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
