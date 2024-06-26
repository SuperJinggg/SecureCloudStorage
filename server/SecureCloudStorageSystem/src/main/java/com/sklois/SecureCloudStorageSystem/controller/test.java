package com.sklois.SecureCloudStorageSystem.controller;

import com.alibaba.fastjson.JSONObject;
import com.aliyuncs.exceptions.ClientException;
import com.sklois.SecureCloudStorageSystem.service.AliDockingCloudStorageSerImpl;
import com.sklois.SecureCloudStorageSystem.service.SwiftDockingCloudStorageSerImpl;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.bind.annotation.RestController;

import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;
import jakarta.servlet.http.HttpSession;
import java.io.IOException;
import java.util.Map;

@RestController
public class test {

    @Autowired
    AliDockingCloudStorageSerImpl aliDockingCloudStorageSer;
    @Autowired
    SwiftDockingCloudStorageSerImpl swiftDockingCloudStorageSer;


    @RequestMapping("/alitest")
    @ResponseBody
    public Object alitest() throws ClientException {
        Map<String, String> ret = aliDockingCloudStorageSer.stsService();
        return ret;
    }

    @RequestMapping("/teststs")
    @ResponseBody
    public Object teststs(@RequestBody JSONObject json, HttpServletRequest request, HttpServletResponse response,
                          HttpSession session) throws ClientException {

        if (null == json) {
            return null;
        }
        String stsaccessid = json.getString("accessKeyId");
        String stsaccesskey = json.getString("accessKeySecret");
        String stsaccesstocken = json.getString("tokenSecret");

        aliDockingCloudStorageSer.putobject(stsaccessid, stsaccesskey, stsaccesstocken);
        return "ssss";
    }

    @RequestMapping("/swifttest")
    @ResponseBody
    public Object swifttest() throws IOException {

        swiftDockingCloudStorageSer.uploaddata(null, "123123");
        return null;
    }
}
