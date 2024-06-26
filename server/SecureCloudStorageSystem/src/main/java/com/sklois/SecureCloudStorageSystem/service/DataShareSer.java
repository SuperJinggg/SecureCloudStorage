package com.sklois.SecureCloudStorageSystem.service;

import java.util.HashMap;

public interface DataShareSer {
    public HashMap<String, Object> sharedata(
            String sessionid,
            String fileuniqueid,
            String shareusername);

    public HashMap<String, Object> getsharelist(String sessionid);
    public HashMap<String, Object> cancelshare(
            String sessionid,
            String fileuniqueid,
            String sharedusername);

    public HashMap<String, Object> getsharedlist(String sessionid);
}
