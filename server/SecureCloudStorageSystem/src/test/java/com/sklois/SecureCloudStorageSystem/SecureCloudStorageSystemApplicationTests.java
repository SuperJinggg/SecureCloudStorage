package com.sklois.SecureCloudStorageSystem;

import com.alibaba.fastjson.JSONObject;
import com.obs.services.ObsClient;
import com.sklois.SecureCloudStorageSystem.entity.AuditEntity;
import com.sklois.SecureCloudStorageSystem.publiccloudstorage.AccessAccount;
import com.sklois.SecureCloudStorageSystem.repository.AuditRepository;
import com.sklois.SecureCloudStorageSystem.repository.RegisterUserRepository;
import com.sklois.SecureCloudStorageSystem.service.DatabaseSerImpl;
import com.sklois.SecureCloudStorageSystem.service.MetadataSer;
import org.apache.tomcat.util.http.fileupload.FileItem;
import org.apache.tomcat.util.http.fileupload.FileItemFactory;
import org.apache.tomcat.util.http.fileupload.disk.DiskFileItemFactory;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.autoconfigure.jdbc.AutoConfigureTestDatabase;
import org.springframework.boot.test.autoconfigure.web.servlet.AutoConfigureMockMvc;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.http.MediaType;
import org.springframework.test.context.junit4.AbstractTransactionalJUnit4SpringContextTests;
import org.springframework.test.context.junit4.SpringRunner;
import org.springframework.test.context.web.WebAppConfiguration;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.test.web.servlet.MvcResult;
import org.springframework.test.web.servlet.request.MockMvcRequestBuilders;

import javax.net.ssl.*;
import java.io.*;
import java.security.*;
import java.security.cert.X509Certificate;
import java.time.LocalDateTime;
import java.util.Base64;
import java.util.UUID;

import static org.junit.Assert.assertEquals;

@RunWith(SpringRunner.class)
@SpringBootTest
@WebAppConfiguration
@AutoConfigureMockMvc
@AutoConfigureTestDatabase(replace = AutoConfigureTestDatabase.Replace.NONE)
public class SecureCloudStorageSystemApplicationTests extends AbstractTransactionalJUnit4SpringContextTests {
    @Test
    public void contextLoads() {
        assertEquals(1, 1);
    }


    @Autowired
    private MetadataSer metadataser;
    @Autowired
    private RegisterUserRepository registeruserrepository;

//    @Test
//    public void contextLoads() {
//        List<JSONObject> folderlist = metadataser.folderlist("normaluser01");
//        System.out.println("folderlist size is " + folderlist.size());
//        System.out.println("folderlist size is " + folderlist.toString());
//    }

//    private Calculator calculator = new Calculator();
//    @Test
//    public void contextLoads() {
//        List<JSONObject> folderlist = metadataser.folderlist("normaluser01");
//        System.out.println("folderlist size is " + folderlist.size());
//        System.out.println("folderlist size is " + folderlist.toString());
//        assertEquals(1,1);
//    }

    @Autowired
    DatabaseSerImpl databaseser;

    @Test
    public void testAlgorithm() {
        String plainttext = UUID.randomUUID().toString().replaceAll("-", "");
        String key = "3b4732938a284fedb5f27e89682e9044";
        byte[] encryptout = databaseser.aesCbcEncrypt(key, plainttext.getBytes());
        String encryptoutbase64 = Base64.getEncoder().encodeToString(encryptout);
        byte[] encryptedBytes = Base64.getDecoder().decode(encryptoutbase64);
        byte[] bytes = databaseser.aesCbcDecrypt(key, encryptedBytes);
        String decrypts = new String(bytes);
        assertEquals(plainttext, decrypts);
    }


//    @Before

    @Autowired
    AuditRepository auditrepository;

    @Test
    public void testRepository() {
        LocalDateTime currentTime = LocalDateTime.now();
        //检查参数是否合法
        AuditEntity auditentity = new AuditEntity();
        auditentity.setTime(currentTime);
        auditentity.setIpaddress("10.10.71.22");
        auditentity.setUsername("normaluser01");
        auditentity.setRole("普通用户");
        auditentity.setMethod("安全审计");
        auditentity.setModule("安全审计模块");
        auditentity.setObject("审计日志查询成功");
        auditentity.setResult("success");
        auditentity.setStatus("终止状态");
        AuditEntity saveresult = auditrepository.save(auditentity);
        assertEquals(auditentity.getIpaddress(), saveresult.getIpaddress());
    }

    @Autowired
    private MockMvc mockmvc;

    @Test
    public void testfolderstructurelist() throws Exception {
        JSONObject param = new JSONObject();
        param.put("method", "folderstructurelist");
        param.put("timestamp", "folderstructurelist");
        param.put("version", "1.0");
        String paraJson = JSONObject.toJSONString(param);
        String sessionid = "7709d6001a33d8cf5f3f65c9350ec05ce9a523daaa8cd9d5cb4a46db52dd5db9";
        MvcResult mvcresult =
                mockmvc.
                        perform(MockMvcRequestBuilders
                                .post("/folderstructurelist")
                                .header("sessionid", sessionid)
                                .contentType(MediaType.APPLICATION_JSON)//请求参数的类型
                                .content(paraJson))
                        .andReturn();
        String ret = mvcresult.getResponse().getContentAsString();
        System.out.println(ret);
    }

    /*
    @Test
    public void testfileupload() throws Exception {

//        String endpoint = BASE_URL + "/upload/photo";

        String sessionid = "1a589d9f11b6c06e985354219273f826fd4a87ae16da86de6f83090863350d6d";
        String  filepath = "/home/ss/F/IdeaProjects/5/SecureCloudStorageSystem/hello.docx";
        FileInputStream fis = new FileInputStream(filepath);
        MockMultipartFile multipartFile = new MockMultipartFile("file", fis);

        MultiValueMap<String, Object> parameters = new LinkedMultiValueMap<String, Object>();
        parameters.add("file", new FileSystemResource(filepath));
        parameters.add("metadata","{}");
        parameters.add("cipherindex","{}");
        parameters.add("encryptkey","xxxxx");

        disableSslVerification();
        RestTemplate restTemplate = new RestTemplate();
        String url = "https://localhost:9443" + "/dataupload";
        HttpHeaders headers = new HttpHeaders();
        headers.set("Content-Type", "multipart/form-data");
        headers.set("sessionid", sessionid);


//        FileSystemResource fileSystemResource = new FileSystemResource(filepath);
//        MultiValueMap<String, Object> form = new LinkedMultiValueMap<>();
//        form.add("name", fileSystemResource);
//        form.add("metadata", "{}");
//        form.add("cipherindex", "{}");
//        form.add("encryptkey", "xxxxx");
//        HttpEntity<MultiValueMap<String, Object>> files = new HttpEntity<>(form, headers);

        String result = restTemplate.postForObject(
                url,
                new HttpEntity<MultiValueMap<String, Object>>(parameters, headers),
                String.class);
        System.out.println("==========结果为：==========\n" + result + "\n");
//        HashMap<String, String> contentTypeParams = new HashMap<String, String>();
//        contentTypeParams.put("boundary", "265001916915724");
//        MediaType mediaType = new MediaType("multipart", "form-data", contentTypeParams);



//        MvcResult mvcresult =
//                mockmvc.
//                        perform(MockMvcRequestBuilders
//                                .post("/dataupload")
//                                .header("sessionid", sessionid)
//                                .contentType("mutipart/form-data;boundary = \"1234567890=\"")//请求参数的类型
//                                .content(multipartFile.getBytes()))
//                        .andReturn();
//        String result = mvcresult.getResponse().getContentAsString();
//        System.out.println("==========结果为：==========\n" + result + "\n");


//        Part part =new Part();


//        MultipartFile part = new MultipartFile();

//        MockMultipartFile part1 = new MockMultipartFile("file",
//                                        "hello.docx",
//                                        "application/octet-stream",
//                                        new FileInputStream(
//                                                new File(
//                                                        "/home/ss/F/IdeaProjects/5/SecureCloudStorageSystem/hello.docx"))


//        ResultActions resultActions =
//                mockmvc.perform(
//                        MockMvcRequestBuilders
//                                .fileUpload("/dataupload")
//                                .file(new MockMultipartFile("file",
//                                        "hello.docx",
//                                        "application/octet-stream",
//                                        new FileInputStream(
//                                                new File(
//                                                        "/home/ss/F/IdeaProjects/5/SecureCloudStorageSystem/hello.docx")))
//                                )
//                                .header("sessionid", sessionid)
//                );
//        MvcResult mvcResult = resultActions
//                .andDo(MockMvcResultHandlers.print())
//                .andExpect(MockMvcResultMatchers.status().isOk()).andReturn();
//        String result = mvcResult.getResponse().getContentAsString();
//        System.out.println("==========结果为：==========\n" + result + "\n");
    }
*/

    private static void disableSslVerification() {
        try {
            // Create a trust manager that does not validate certificate chains
            TrustManager[] trustAllCerts = new TrustManager[]{new X509TrustManager() {
                public java.security.cert.X509Certificate[] getAcceptedIssuers() {
                    return null;
                }

                public void checkClientTrusted(X509Certificate[] certs, String authType) {
                }

                public void checkServerTrusted(X509Certificate[] certs, String authType) {
                }
            }};

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


    @Autowired
    private HuaweiDockingCloudStorageSerImpl huaweicloudstorageser;

    static String endPoint = AccessAccount.huaweiendpoint;
    static String ak = AccessAccount.huaweiaccessKeyId;
    static String sk = AccessAccount.huaweiaccessKeySecret;
    static String bucketname = "beijing2";


//    @Test
//    public void testhuawei() {
//        String fileuniqueid = "1234567890123456789012345678901234567890";
//        File file = new File("hello.decx");
//        ObsClient obsClient = new ObsClient(ak, sk, endPoint);
//        InputStream in = new FileInputStream(file);
//        obsClient.putObject(bucketname, fileuniqueid,
//                file.getInputStream());
//
//    }


    // 调用
    private FileItem getMultipartFile(File file, String fieldName) {
        FileItemFactory factory = new DiskFileItemFactory(16, null);
        FileItem item = factory.createItem(fieldName, "text/plain", true, file.getName());
        int bytesRead = 0;
        int len = 8192;
        byte[] buffer = new byte[len];
        try {
            FileInputStream fis = new FileInputStream(file);
            OutputStream os = item.getOutputStream();
            while ((bytesRead = fis.read(buffer, 0, len)) != -1) {
                os.write(buffer, 0, bytesRead);
            }
            os.close();
            fis.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return item;
    }
}
