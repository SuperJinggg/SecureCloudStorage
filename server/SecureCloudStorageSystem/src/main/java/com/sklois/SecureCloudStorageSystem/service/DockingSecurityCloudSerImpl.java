package com.sklois.SecureCloudStorageSystem.service;

import com.alibaba.fastjson.JSONArray;
import com.alibaba.fastjson.JSONObject;
import com.sklois.SecureCloudStorageSystem.entity.RegisterUser;
import com.sklois.SecureCloudStorageSystem.repository.RegisterUserRepository;
import com.sklois.SecureCloudStorageSystem.securitycloud.SecurityCloudServiceAccessAccount;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpMethod;
import org.springframework.http.MediaType;
import org.springframework.stereotype.Service;
import org.springframework.web.client.RestTemplate;

import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;
import javax.net.ssl.*;
import java.security.*;
import java.security.cert.X509Certificate;
import java.security.spec.AlgorithmParameterSpec;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.Base64;



@Service
public class DockingSecurityCloudSerImpl implements DockingSecurityCloudSer {
    @Value("${sklois.securitycloud.keyencryptkey}")
    private String keyencryptkey;
    @Autowired
    private RegisterUserRepository registeruserrepository;
    private String securitycloudaccessid =
            SecurityCloudServiceAccessAccount.securitycloudaccesscredidentid;
    private String securitycloudaccesskey =
            SecurityCloudServiceAccessAccount.securitycloudaccesscredidentkey;
    private String securitycloudaccessendpoint =
            SecurityCloudServiceAccessAccount.securitycloudendpoint;


    @Override
    public String uploadkey(String sessionid,String encryptkey,String fileuniqueid) {
        byte[] encryptedBytes = Base64.getDecoder().decode(encryptkey);
        RegisterUser findout = registeruserrepository.findOneBySessionid(sessionid);
        byte[] bytes = aesCbcDecrypt(findout.getSessionkey(), encryptedBytes);
        String decryptkey = new String(bytes);
        System.out.println("decryptkey is "+decryptkey);

        byte[] encryptout = aesCbcEncrypt(keyencryptkey , decryptkey.getBytes());
        String encryptoutbase64 = Base64.getEncoder().encodeToString(encryptout);
        //upload
        disableSslVerification();
        RestTemplate restTemplate = new RestTemplate();
        String url = securitycloudaccessendpoint + "/uploadkey";
        HttpHeaders headers = new HttpHeaders();
        headers.add("accessid", securitycloudaccessid);
        headers.add("accesskey", securitycloudaccesskey);
        MediaType type = MediaType.parseMediaType("application/json; charset=UTF-8");
        headers.setContentType(type);

        JSONObject msgjsonobj = new JSONObject();
        JSONObject requestjsonobj = new JSONObject();
        requestjsonobj.put("fileuniqueid",fileuniqueid);
        requestjsonobj.put("fileencryptkey",encryptoutbase64);
        msgjsonobj.put("method","uploadkey");
        msgjsonobj.put("request",requestjsonobj);
        LocalDateTime currentTime = LocalDateTime.now();
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
        msgjsonobj.put("timestamp",currentTime.format(formatter));
        msgjsonobj.put("version","1.0");
        HttpEntity entity = new HttpEntity(msgjsonobj.toString(), headers);
        String strbody = restTemplate.exchange(url, HttpMethod.POST, entity, String.class).getBody();
        HttpHeaders responseheaders = new HttpHeaders();
        JSONObject jsonObject = new JSONObject();
        jsonObject = JSONObject.parseObject(strbody);
        System.out.println("strbody is "+strbody);
        return strbody;
    }
    @Override
    public String getkey(String sessionid ,String fileuniqueid){
        //getkey
        disableSslVerification();
        RestTemplate restTemplate = new RestTemplate();
        String url = securitycloudaccessendpoint + "/getkey";
        HttpHeaders headers = new HttpHeaders();
        headers.add("accessid", securitycloudaccessid);
        headers.add("accesskey", securitycloudaccesskey);
        MediaType type = MediaType.parseMediaType("application/json; charset=UTF-8");
        headers.setContentType(type);

        JSONObject msgjsonobj = new JSONObject();
        JSONObject requestjsonobj = new JSONObject();
        requestjsonobj.put("fileuniqueid",fileuniqueid);
        msgjsonobj.put("method","getkey");
        msgjsonobj.put("request",requestjsonobj);
        LocalDateTime currentTime = LocalDateTime.now();
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
        msgjsonobj.put("timestamp",currentTime.format(formatter));
        msgjsonobj.put("version","1.0");
        HttpEntity entity = new HttpEntity(msgjsonobj.toString(), headers);
        String strbody = restTemplate.exchange(url, HttpMethod.POST, entity, String.class).getBody();
        JSONObject jsonObject = new JSONObject();
        jsonObject = JSONObject.parseObject(strbody);

        JSONObject details = jsonObject.getJSONObject("details");
        String getfileencryptkey = details.getString("fileencryptkey");
        String getfileuniqueid = details.getString("fileuniqueid");
        //解密
        byte[] fileencryptkeyBytes = Base64.getDecoder().decode(getfileencryptkey);
        byte[] decreptkey = aesCbcDecrypt(keyencryptkey,fileencryptkeyBytes);
        String decryptkeystr = new String(decreptkey);
        System.out.println("decryptkeystr is "+decryptkeystr);


        RegisterUser findout = registeruserrepository.findOneBySessionid(sessionid);
        byte[] encryptout = aesCbcEncrypt(findout.getSessionkey() , decreptkey);
        String encryptoutbase64 = Base64.getEncoder().encodeToString(encryptout);
        return encryptoutbase64;
    }

    public String deletekey(String fileuniqueid){
        disableSslVerification();
        RestTemplate restTemplate = new RestTemplate();
        String url = securitycloudaccessendpoint + "/deletekey";
        HttpHeaders headers = new HttpHeaders();
        headers.add("accessid", securitycloudaccessid);
        headers.add("accesskey", securitycloudaccesskey);
        MediaType type = MediaType.parseMediaType("application/json; charset=UTF-8");
        headers.setContentType(type);

        JSONObject msgjsonobj = new JSONObject();
        JSONObject requestjsonobj = new JSONObject();
        requestjsonobj.put("fileuniqueid",fileuniqueid);
        msgjsonobj.put("method","deletekey");
        msgjsonobj.put("request",requestjsonobj);
        LocalDateTime currentTime = LocalDateTime.now();
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
        msgjsonobj.put("timestamp",currentTime.format(formatter));
        msgjsonobj.put("version","1.0");
        HttpEntity entity = new HttpEntity(msgjsonobj.toString(), headers);
        String strbody = restTemplate.exchange(url, HttpMethod.POST, entity, String.class).getBody();
        System.out.println("strbody is "+strbody);
        return strbody;
    }
    @Override
    public String uploadcipherindex(JSONObject cipherindexjsonobj) {
        disableSslVerification();
        RestTemplate restTemplate = new RestTemplate();
        String url = securitycloudaccessendpoint + "/uploadcipherindex";
        HttpHeaders headers = new HttpHeaders();
        headers.add("accessid", securitycloudaccessid);
        headers.add("accesskey", securitycloudaccesskey);
        MediaType type = MediaType.parseMediaType("application/json; charset=UTF-8");
        headers.setContentType(type);
        JSONObject msgjsonobj = new JSONObject();
        msgjsonobj.put("method","uploadcipherindex");
        msgjsonobj.put("request",cipherindexjsonobj.getJSONObject("retdata"));
        LocalDateTime currentTime = LocalDateTime.now();
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
        msgjsonobj.put("timestamp",currentTime.format(formatter));
        msgjsonobj.put("version","1.0");
        HttpEntity entity = new HttpEntity(msgjsonobj.toString(), headers);
        String strbody = restTemplate.exchange(url, HttpMethod.POST, entity, String.class).getBody();
        System.out.println("strbody is "+strbody);
        return strbody;
    }

    public String searchcipherindex(String condition, JSONArray keywords) {
        disableSslVerification();
        RestTemplate restTemplate = new RestTemplate();
        String url = securitycloudaccessendpoint + "/searchcipherindex";
        HttpHeaders headers = new HttpHeaders();
        headers.add("accessid", securitycloudaccessid);
        headers.add("accesskey", securitycloudaccesskey);
        MediaType type = MediaType.parseMediaType("application/json; charset=UTF-8");
        headers.setContentType(type);
        JSONObject msgjsonobj = new JSONObject();
        JSONObject requestjsonobj = new JSONObject();
        msgjsonobj.put("method","searchcipherindex");
        requestjsonobj.put("condition",condition);
        requestjsonobj.put("keywords",keywords);
        msgjsonobj.put("request",requestjsonobj);
        LocalDateTime currentTime = LocalDateTime.now();
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
        msgjsonobj.put("timestamp",currentTime.format(formatter));
        msgjsonobj.put("version","1.0");
        HttpEntity entity = new HttpEntity(msgjsonobj.toString(), headers);
        String strbody = restTemplate.exchange(url, HttpMethod.POST, entity, String.class).getBody();
        System.out.println("strbody is "+strbody);
        return strbody;
    }

    public String deletecipherindex(String fileuniqueid) {
        disableSslVerification();
        RestTemplate restTemplate = new RestTemplate();
        String url = securitycloudaccessendpoint + "/deletecipherindex";
        HttpHeaders headers = new HttpHeaders();
        headers.add("accessid", securitycloudaccessid);
        headers.add("accesskey", securitycloudaccesskey);
        MediaType type = MediaType.parseMediaType("application/json; charset=UTF-8");
        headers.setContentType(type);
        JSONObject msgjsonobj = new JSONObject();
        JSONObject requestjsonobj = new JSONObject();
        msgjsonobj.put("method","deletecipherindex");
        requestjsonobj.put("fileuniqueid",fileuniqueid);
        msgjsonobj.put("request",requestjsonobj);
        LocalDateTime currentTime = LocalDateTime.now();
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
        msgjsonobj.put("timestamp",currentTime.format(formatter));
        msgjsonobj.put("version","1.0");
        HttpEntity entity = new HttpEntity(msgjsonobj.toString(), headers);
        String strbody = restTemplate.exchange(url, HttpMethod.POST, entity, String.class).getBody();
        System.out.println("strbody is "+strbody);
        return strbody;
    }

    public String auditloglist(String fromtime,String totime,String module,int pagenum,int pagesize) {
        disableSslVerification();
        RestTemplate restTemplate = new RestTemplate();
        String url = securitycloudaccessendpoint + "/auditloglist";
        HttpHeaders headers = new HttpHeaders();
        headers.add("accessid", securitycloudaccessid);
        headers.add("accesskey", securitycloudaccesskey);
        MediaType type = MediaType.parseMediaType("application/json; charset=UTF-8");
        headers.setContentType(type);
        JSONObject msgjsonobj = new JSONObject();
        JSONObject requestjsonobj = new JSONObject();
        msgjsonobj.put("method","auditloglist");
        requestjsonobj.put("fromtime",fromtime);
        requestjsonobj.put("totime",totime);
        requestjsonobj.put("module",module);
        requestjsonobj.put("pagenum",pagenum);
        requestjsonobj.put("pagesize",pagesize);
        msgjsonobj.put("request",requestjsonobj);
        LocalDateTime currentTime = LocalDateTime.now();
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
        msgjsonobj.put("timestamp",currentTime.format(formatter));
        msgjsonobj.put("version","1.0");
        HttpEntity entity = new HttpEntity(msgjsonobj.toString(), headers);
        String strbody = restTemplate.exchange(url, HttpMethod.POST, entity, String.class).getBody();
        System.out.println("strbody is "+strbody);
        return strbody;
    }


    public byte[] aesCbcEncrypt(String key, byte[] input) {
        Cipher cipher = null;
        byte[] cipherout = null;
        try {
            cipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
            Key secretKey = new SecretKeySpec(key.getBytes(), "AES");
            byte[] iv = new byte[cipher.getBlockSize()];
            AlgorithmParameterSpec algorithmParameters = new IvParameterSpec(iv);
            cipher.init(Cipher.ENCRYPT_MODE, secretKey, algorithmParameters);
            cipherout = cipher.doFinal(input);
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        } catch (NoSuchPaddingException e) {
            e.printStackTrace();
        } catch (InvalidKeyException e) {
            e.printStackTrace();
        } catch (InvalidAlgorithmParameterException e) {
            e.printStackTrace();
        } catch (BadPaddingException e) {
            e.printStackTrace();
        } catch (IllegalBlockSizeException e) {
            e.printStackTrace();
        }
        return cipherout;
    }
    public byte[] aesCbcDecrypt(String key, byte[] input) {
        Cipher cipher = null;
        byte[] cipherout = null;
        try {
            cipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
            Key secretKey = new SecretKeySpec(key.getBytes(), "AES");
            byte[] iv = new byte[cipher.getBlockSize()];
            AlgorithmParameterSpec algorithmParameters = new IvParameterSpec(iv);
            cipher.init(Cipher.DECRYPT_MODE, secretKey, algorithmParameters);
            cipherout = cipher.doFinal(input);
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        } catch (NoSuchPaddingException e) {
            e.printStackTrace();
        } catch (InvalidKeyException e) {
            e.printStackTrace();
        } catch (InvalidAlgorithmParameterException e) {
            e.printStackTrace();
        } catch (BadPaddingException e) {
            e.printStackTrace();
        } catch (IllegalBlockSizeException e) {
            e.printStackTrace();
        }
        return cipherout;
    }

    private static void disableSslVerification() {
        try {
            // Create a trust manager that does not validate certificate chains
            TrustManager[] trustAllCerts = new TrustManager[] { new X509TrustManager() {
                public java.security.cert.X509Certificate[] getAcceptedIssuers() {
                    return null;
                }

                public void checkClientTrusted(X509Certificate[] certs, String authType) {
                }

                public void checkServerTrusted(X509Certificate[] certs, String authType) {
                }
            } };

            // Install the all-trusting trust manager
            SSLContext sc = SSLContext.getInstance("SSL");
            sc.init(null, trustAllCerts, new java.security.SecureRandom());
            HttpsURLConnection.setDefaultSSLSocketFactory(sc.getSocketFactory());

            // Create all-trusting host name verifier
            HostnameVerifier allHostsValid = new HostnameVerifier() {
                public boolean verify(String hostname, SSLSession session) {
                    return true;
                }
            };

            // Install the all-trusting host verifier
            HttpsURLConnection.setDefaultHostnameVerifier(allHostsValid);
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        } catch (KeyManagementException e) {
            e.printStackTrace();
        }
    }
}
