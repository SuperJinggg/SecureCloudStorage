package com.sklois.SecureCloudStorageSystem.service;

import com.alibaba.fastjson.JSONObject;
import org.springframework.core.io.ByteArrayResource;
import org.springframework.core.io.FileSystemResource;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpMethod;
import org.springframework.http.MediaType;
import org.springframework.stereotype.Service;
import org.springframework.util.LinkedMultiValueMap;
import org.springframework.util.MultiValueMap;
import org.springframework.web.client.RestTemplate;
import org.springframework.web.multipart.MultipartFile;

import java.io.*;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

@Service
public class SwiftDockingCloudStorageSerImpl implements DockingCloudStorageSer {
    private String username = "user2";
    private String password = "qwertyuiop!@#123";
    String tokenurl = "http://10.10.102.151:5000/v3/auth/tokens";
    String objectstorageurl = "http://10.10.102.151:8080/v1/AUTH_77b973f8dab344439505b6c1f20fb6dd";
    String bucketname = "testcontainer";

    @Override
    public void uploaddata(MultipartFile file, String fileuniqueid) throws IOException {
//        System.setProperty("http.proxyHost", "127.0.0.1");
//        System.setProperty("https.proxyHost", "127.0.0.1");
//        System.setProperty("http.proxyPort", "8888");
//        System.setProperty("https.proxyPort", "8888");

        String token = gettoken();
        RestTemplate uploadfilerestTemplate = new RestTemplate();
        HttpHeaders uploadfileheaders = new HttpHeaders();
        uploadfileheaders.setContentType(MediaType.MULTIPART_FORM_DATA);
        uploadfileheaders.add("X-Auth-Token",token);
        MultiValueMap<String, Object> param = new LinkedMultiValueMap<>();
        ByteArrayResource conetentsasresource = new ByteArrayResource(file.getBytes()){
          @Override
          public String getFilename(){
              return file.getOriginalFilename();
          }
        };
        param.add("file", conetentsasresource);
//        MultiValueMap<String, Object> multipartRequest = new LinkedMultiValueMap<>();
//        multipartRequest.add("file", file);
        HttpEntity<MultiValueMap<String, Object>> uploadentity = new HttpEntity<>(param, uploadfileheaders);
        String strbody = uploadfilerestTemplate.exchange(
                objectstorageurl+"/"+bucketname+"/"+fileuniqueid,
                HttpMethod.PUT,
                uploadentity, String.class).getBody();
        System.out.println("strbody is " + strbody);
    }

    @Override
    public InputStream downloaddata(String fileuniqueid) {
        String token = gettoken();
        RestTemplate downloadrestTemplate = new RestTemplate();
        HttpHeaders downloadheaders = new HttpHeaders();
        InputStream inputStream = null;
        String downloadbodyValTemplate = "";
        HttpEntity downloadentity = new HttpEntity(downloadbodyValTemplate, downloadheaders);
        byte[] strbody = downloadrestTemplate.exchange(objectstorageurl+"/"+bucketname+"/"+fileuniqueid,
                HttpMethod.GET, downloadentity, byte[].class).getBody();
        if (null != strbody && strbody.length > 0) {
            ByteArrayInputStream bis = new ByteArrayInputStream(strbody);
            inputStream = new BufferedInputStream(bis);
        }
        return inputStream;
    }

    @Override
    public void deletedata(String fileuniqueid) {
        String token = gettoken();
        RestTemplate deleterestTemplate = new RestTemplate();
        HttpHeaders deleteheaders = new HttpHeaders();
        String deletebodyValTemplate = "";
        HttpEntity deleteentity = new HttpEntity(deletebodyValTemplate, deleteheaders);
        String strbody = deleterestTemplate.exchange(objectstorageurl+"/"+bucketname+"/"+fileuniqueid,
                HttpMethod.DELETE, deleteentity, String.class).getBody();
        System.out.println("strbody is " + strbody);
    }

    /**
     * 将字节流转换成文件
     *
     * @param filename
     * @param data
     * @throws Exception
     */
    public static void saveFile(String filename, byte[] data) throws Exception {
        if (data != null) {
            String filepath = filename;
            File file = new File(filepath);
            if (file.exists()) {
                file.delete();
            }
            FileOutputStream fos = new FileOutputStream(file);
            fos.write(data, 0, data.length);
            fos.flush();
            fos.close();
        }
    }

    public Map<String, String> stsService(){
        String token = gettoken();
        System.out.println("token is " + token);
        Map<String, String> result = new HashMap<String, String>();
        result.put("token", token);
        return result;
    }

    public String gettoken(){
        RestTemplate restTemplate = new RestTemplate();
        HttpHeaders headers = new HttpHeaders();
        headers.setContentType(MediaType.APPLICATION_JSON);
        String bodyValTemplate = "{\"auth\": {\"identity\": {\"methods\": [\"password\"],\"password\": {\"user\": {\"name\": \"" + username + "\",\"domain\": {\"name\": \"Default\"},\"password\":\"" + password + "\"}}}}}";
        HttpEntity entity = new HttpEntity(bodyValTemplate, headers);
        HttpHeaders httpheads = restTemplate.exchange(tokenurl,
                HttpMethod.POST,
                entity,
                String.class).getHeaders();
        List<String> tokenlist = httpheads.get("x-subject-token");
        String token = null;

        if(null!=tokenlist&&tokenlist.size()>0){
            token = tokenlist.get(0);
        }
        System.out.println("token is " + token);
        return token;
    }
}
