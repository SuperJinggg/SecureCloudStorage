package com.sklois.SecureCloudStorageSystem.service;

import com.aliyun.oss.ClientException;
import com.aliyun.oss.OSS;
import com.aliyun.oss.OSSClientBuilder;
import com.aliyun.oss.model.GetObjectRequest;
import com.aliyun.oss.model.OSSObject;
import com.aliyun.oss.model.PutObjectResult;
import com.aliyuncs.DefaultAcsClient;
import com.aliyuncs.auth.sts.AssumeRoleRequest;
import com.aliyuncs.auth.sts.AssumeRoleResponse;
import com.aliyuncs.http.MethodType;
import com.aliyuncs.profile.DefaultProfile;
import com.aliyuncs.profile.IClientProfile;
import com.sklois.SecureCloudStorageSystem.publiccloudstorage.AccessAccount;
import org.springframework.stereotype.Service;
import org.springframework.util.FileCopyUtils;
import org.springframework.web.multipart.MultipartFile;

import java.io.*;
import java.util.HashMap;
import java.util.Map;

@Service
public class AliDockingCloudStorageSerImpl implements DockingCloudStorageSer {

    static String endpoint = AccessAccount.aliendpoint;
    // 阿里云主账号AccessKey拥有所有API的访问权限，风险很高。强烈建议您创建并使用RAM账号进行API访问或日常运维，请登录
    // https://ram.console.aliyun.com 创建RAM账号。
    static String accessKeyId = AccessAccount.aliaccessKeyId;
    static String accessKeySecret = AccessAccount.aliaccessKeySecret;
    static String bucketname = AccessAccount.bucketname;
    @Override
    public void uploaddata(MultipartFile file,String fileuniqueid) throws IOException {
        OSS ossClient = new OSSClientBuilder().build(endpoint, accessKeyId, accessKeySecret);
        PutObjectResult saveresult = ossClient.putObject(
                bucketname, fileuniqueid, file.getInputStream());
        ossClient.shutdown();
        System.out.println("saveresult.toString() is"+saveresult.toString());
    }

    @Override
    public InputStream downloaddata(String fileuniqueid) {
        OSS ossClient = new OSSClientBuilder().build(endpoint, accessKeyId, accessKeySecret);
        // 下载OSS文件到本地文件。如果指定的本地文件存在会覆盖，不存在则新建。
        OSSObject getobject = ossClient.getObject(bucketname, fileuniqueid);
        InputStream is = getobject.getObjectContent();
        /* 关闭OSSClient。 */
//        ossClient.shutdown();
        return is;
    }

    @Override
    public void deletedata(String fileuniqueid) {
        OSS ossClient = new OSSClientBuilder().build(endpoint, accessKeyId, accessKeySecret);
        // 下载OSS文件到本地文件。如果指定的本地文件存在会覆盖，不存在则新建。
        ossClient.deleteObject(bucketname, fileuniqueid);
        // 关闭OSSClient。
        ossClient.shutdown();
    }


    public Map<String, String> stsService() throws ClientException, com.aliyuncs.exceptions.ClientException {
        // 只有 RAM用户（子账号）才能调用 AssumeRole 接口 .
        // 阿里云主账号的AccessKeys不能用于发起AssumeRole请求
        // 首先在RAM控制台创建一个RAM用户，并为这个用户创建AccessKeys
        // 扮演角色的时候额外加上的一个权限限制，写法和授权策略一样
        String policy = null;
        // 可以为空 是一个用来标示临时凭证的名称，一般来说建议使用不同的应用程序用户来区分
        String roleSessionName = "roleUser";
        // 临时凭证的有效期，单位是s，最小为900，最大为3600
        long sessionTime = 3600;
        DefaultProfile.addEndpoint("", "", "Sts", endpoint);
        IClientProfile profile = DefaultProfile.getProfile("cn-beijing",
                AccessAccount.alistsaccessKeyId,
                AccessAccount.alistsaccessKeySecret);

        // 用profile构造client
        DefaultAcsClient client = new DefaultAcsClient(profile);
        final AssumeRoleRequest request = new AssumeRoleRequest();
        request.setMethod(MethodType.POST);
        // 临时Token的会话名称，自己指定用于标识你的用户，主要用于区分Token颁发给谁
        // acs:ram::$accountID:role/$roleName

        request.setRoleArn("acs:ram::1115905405499911:role/aliyunosstokengeneratorrole");
        request.setRoleSessionName("external-username");
//        request.setPolicy(policy);
        request.setDurationSeconds(sessionTime);
        // 发起请求，并得到response
        AssumeRoleResponse response = client.getAcsResponse(request);
        System.out.println("Expiration: " + response.getCredentials().getExpiration());
        System.out.println("Access Key Id: " + response.getCredentials().getAccessKeyId());
        System.out.println("Access Key Secret: " + response.getCredentials().getAccessKeySecret());
        System.out.println("Security Token: " + response.getCredentials().getSecurityToken());
        System.out.println("RequestId: " + response.getRequestId());
        Map<String, String> result = new HashMap<String, String>();
        result.put("accessKeyId", response.getCredentials().getAccessKeyId());
        result.put("accessKeySecret", response.getCredentials().getAccessKeySecret());
        result.put("tokenSecret", response.getCredentials().getSecurityToken());


        return result;
    }

    public void putobject(String stsaccessid, String stsaccesskey,String stsaccesstocken){
        OSS ossClient = new OSSClientBuilder().build(endpoint, stsaccessid, stsaccesskey,stsaccesstocken);



//        PutObjectResult saveresult = ossClient.putObject(
//                bucketname, "123", file.getInputStream());

        PutObjectResult saveresult = ossClient.putObject(
                bucketname, "HELP.md", new File("./HELP.md"));

        ossClient.shutdown();
        System.out.println("saveresult.toString() is"+saveresult.toString());
    }



}
