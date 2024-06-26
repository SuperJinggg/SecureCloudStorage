package com.sklois.SecureCloudStorageSystem.service;

import com.alibaba.fastjson.JSONArray;
import com.alibaba.fastjson.JSONObject;

public interface DockingSecurityCloudSer {


    public String uploadkey(String sessionid ,String encryptkey,String fileuniqueid);
    public String getkey(String sessionid ,String fileuniqueid);
    public String deletekey(String fileuniqueid);
    public String uploadcipherindex(JSONObject cipherindex);
    public String searchcipherindex(String condition, JSONArray keywords);
}
