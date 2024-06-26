package com.sklois.SecureCloudStorageSystem.service;

import com.sklois.SecureCloudStorageSystem.entity.RegisterUser;
import org.springframework.stereotype.Service;

import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.time.LocalDateTime;
import java.util.Date;
import java.util.HashMap;
import java.util.List;


public interface DatabaseSer {
    public HashMap<String,Object> findByName(String name);
    public HashMap<String,Object> save(String name, String password, String role, LocalDateTime date);
    public HashMap<String,Object> findAll();

    public Object findOneByName(String name);
    public boolean macverity(RegisterUser registeruser,String username,String authcode,String random,String timestamp,String mac) throws NoSuchAlgorithmException, InvalidKeyException;

    public RegisterUser savesessioninfo(
            RegisterUser registeruser,String sessionid,
            String sessionkey,
            LocalDateTime currenttime,String ipaddress);
    public HashMap<String, Object> verifysessionid(String sessionid);
    public RegisterUser userlogout(String sessionkey);

    public Object modifyuser(String username , String email);
    public Object deleteuser(String username);
}
