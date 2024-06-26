package com.sklois.SecureCloudStorageSystem.service;

import com.sklois.SecureCloudStorageSystem.entity.RegisterUser;
import com.sklois.SecureCloudStorageSystem.repository.RegisterUserRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Service;

import javax.crypto.*;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;
import java.io.UnsupportedEncodingException;
import java.security.*;
import java.security.spec.AlgorithmParameterSpec;
import java.time.Duration;
import java.time.LocalDateTime;
import java.util.*;


@Service
public class DatabaseSerImpl implements DatabaseSer {
    String lowercase = "^.*[a-z]+.*$";
    String uppercase = "^.*[A-Z]+.*$";
    String numbercase = "^.*[0-9]+.*$";
    String usernameRange = "^[a-zA-Z0-9]+$";
    @Autowired
    private RegisterUserRepository registeruserrepository;


    @Value("${sklois.auth.inactiveinterval}")
    private long inactiveinterval;
    @Value("${sklois.database.saltvalue}")
    private String saltvalue;
    @Value("${sklois.database.encryption.key}")
    private String databaseencryptkey;




    @Override
    public HashMap<String, Object> findByName(String name) {
        HashMap<String, Object> retmap = new HashMap<String, Object>();
        List<RegisterUser> findout = new ArrayList<RegisterUser>();
        if (name.length() < 8 || name.length() > 20) {
            retmap.put("result", false);
            retmap.put("message", "username length error");
            return retmap;
        }
        if (!name.matches(usernameRange)) {
            retmap.put("result", false);
            retmap.put("message", "username error");
            return retmap;
        }
        if (!name.matches(lowercase) &&
                !name.matches(uppercase)) {
            retmap.put("result", false);
            retmap.put("message", "username error");
            return retmap;
        }
        if (!name.matches(numbercase)) {
            retmap.put("result", false);
            retmap.put("message", "username error");
            return retmap;
        }
        findout = registeruserrepository.findByName(name);
        retmap.put("result", true);
        retmap.put("message", "success");
        retmap.put("findout", findout);
        return retmap;
    }



    public HashMap<String, Object> save(String name, String password, String role, LocalDateTime date) {
        HashMap<String, Object> retmap = new HashMap<String, Object>();
        if (name.length() < 8 || name.length() > 20) {
            retmap.put("result", false);
            retmap.put("message", "username length error");
            return retmap;
        }
        if (!name.matches(usernameRange)) {
            retmap.put("result", false);
            retmap.put("message", "username error");
            return retmap;
        }
        if (!name.matches(lowercase) &&
                !name.matches(uppercase)) {
            retmap.put("result", false);
            retmap.put("message", "username error");
            return retmap;
        }
        if (!name.matches(numbercase)) {
            retmap.put("result", false);
            retmap.put("message", "username error");
            return retmap;
        }
        if (password.length() != 44) {
            retmap.put("result", false);
            retmap.put("message", "password error");
            return retmap;
        }
        if (null == role || (0 != role.compareTo("安全保密管理员")
                && 0 != role.compareTo("系统管理员")
                && 0 != role.compareTo("安全审计员")
                && 0 != role.compareTo("普通用户"))) {
            retmap.put("result", false);
            retmap.put("message", "role error");
            return retmap;
        }

        System.out.println("password is " + password);
        System.out.println("saltvalue is " + saltvalue);

        List<RegisterUser> findout = new ArrayList<RegisterUser>();
        //查询数据库是否存在
        findout = registeruserrepository.findByName(name);
        if (null != findout && findout.size() > 0) {
            retmap.put("result", false);
            retmap.put("message", "username exist");
            return retmap;
        }


        MessageDigest messageDigest;

        byte[] savedigest = new byte[0];
        try {
            messageDigest = MessageDigest.getInstance("SHA-256");
            messageDigest.update((saltvalue + password).getBytes("UTF-8"));
            savedigest = messageDigest.digest();
        } catch (NoSuchAlgorithmException e) {
            System.out.println("encode password error");
            e.printStackTrace();
        } catch (UnsupportedEncodingException e) {
            System.out.println("encode password error");
            e.printStackTrace();
        }
        String encryptpassword = null;
        byte[] encryptout = aesCbcEncrypt(savedigest);
        encryptpassword = Base64.getEncoder().encodeToString(encryptout);
        System.out.println("encrypt password is " + encryptpassword);

        LocalDateTime currentTime = LocalDateTime.now();
        RegisterUser registeruser = new RegisterUser();
        registeruser.setName(name);
        registeruser.setPassword(encryptpassword);
        registeruser.setRole(role);
        registeruser.setRegisterdate(currentTime);

//        registeruser.set

        RegisterUser retentity = registeruserrepository.save(registeruser);

        if (null != retentity) {
            retmap.put("result", true);
            retmap.put("message", "user register success");
            retmap.put("username", retentity.getName());
            retmap.put("role", retentity.getRole());
            return retmap;
        } else {
            retmap.put("result", false);
            retmap.put("message", "save error");
            return retmap;
        }


    }

    public Object findOneByName(String name) {
        RegisterUser registeruser = registeruserrepository.findOneByName(name);
        return registeruser;
    }


    public byte[] aesCbcEncrypt(byte[] input) {
        Cipher cipher = null;
        byte[] cipherout = null;
        try {
            cipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
            Key secretKey = new SecretKeySpec(databaseencryptkey.getBytes(), "AES");
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
    public byte[] aesCbcEncrypt(String key ,byte[] input) {
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

    public byte[] aesCbcDecrypt(byte[] input) {
        Cipher cipher = null;
        byte[] cipherout = null;
        try {
            cipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
            Key secretKey = new SecretKeySpec(databaseencryptkey.getBytes(), "AES");
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
    public byte[] aesCbcDecrypt(String key ,byte[] input) {
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

    public HashMap<String, Object> findAll() {

        HashMap<String, Object> findoutmap = new HashMap<String, Object>();
        List<RegisterUser> findout = registeruserrepository.findAll();

        if (null != findout) {
            findoutmap.put("result", true);
            findoutmap.put("message", "user find success");
            findoutmap.put("findout", findout);
            return findoutmap;
        }
        return findoutmap;
    }


    /**
     * @param registeruser
     * @param username
     * @param authcode
     * @param random
     * @param mac
     * @return
     * @throws NoSuchAlgorithmException
     * @throws InvalidKeyException
     */
    public boolean macverity(RegisterUser registeruser, String username, String authcode, String random, String timestamp, String mac) throws NoSuchAlgorithmException, InvalidKeyException {
        //解密
        byte[] encryptedBytes = Base64.getDecoder().decode(registeruser.getPassword());
        byte[] decreptkey = aesCbcDecrypt(encryptedBytes);
        String decreptkeystr = byteArrayToHexString(decreptkey);
        System.out.println("decreptkeystr is " + decreptkeystr);
        Mac sha256_HMAC = Mac.getInstance("HmacSHA256");
        String message = username + "&" + authcode.toLowerCase() + "&" + random + "&" + timestamp;
        SecretKeySpec secret_key = new SecretKeySpec(decreptkey, "HmacSHA256");
        sha256_HMAC.init(secret_key);
        byte[] newmac = sha256_HMAC.doFinal(message.getBytes());
        byte[] decodemac = Base64.getDecoder().decode(mac);
        if (Arrays.equals(newmac, decodemac)) {
            System.out.println("√√√√√");
            return true;
        } else {
            System.out.println("xxxxxx");
            return false;
        }
    }

    private static String byteArrayToHexString(byte[] b) {
        StringBuilder hs = new StringBuilder();
        String stmp;
        for (int n = 0; b != null && n < b.length; n++) {
            stmp = Integer.toHexString(b[n] & 0XFF);
            if (stmp.length() == 1)
                hs.append('0');
            hs.append(stmp);
        }
        return hs.toString().toLowerCase();
    }


    public RegisterUser savesessioninfo(RegisterUser registeruser,String sessionid,
                                        String sessionkey, LocalDateTime currenttime,String ipaddress){
        registeruser.setSessionid(sessionid);
        registeruser.setSessionkey(sessionkey);
        registeruser.setSessiontime(currenttime);
        registeruser.setLastauthtime(currenttime);
        registeruser.setLastauthipaddress(ipaddress);
        RegisterUser saveresult = registeruserrepository.save(registeruser);
        return saveresult;
    }

    public HashMap<String, Object> verifysessionid(String sessionid){
        HashMap<String, Object> verifymap = new HashMap<String, Object>();
        RegisterUser findout = registeruserrepository.findOneBySessionid(sessionid);
        if (null != findout&&null!=findout.getSessionid()&&findout.getSessionid().length()>0) {
            LocalDateTime currentTime = LocalDateTime.now();
            LocalDateTime sessiontime = findout.getSessiontime();
            Duration duration = Duration.between(sessiontime,  currentTime);
            if(duration.toMillis()/1000<=inactiveinterval){
                verifymap.put("result", true);
                findout.setSessiontime(currentTime);
                registeruserrepository.save(findout);
                verifymap.put("entity",findout);
            }else{
                verifymap.put("result", false);
            }
            return verifymap;
        }else{
            verifymap.put("result", false);
            return verifymap;
        }
    }

    public Object modifyuser(String username , String role) {
        RegisterUser registeruser = registeruserrepository.findOneByName(username);
        if(null!=registeruser){
            registeruser.setRole(role);
            RegisterUser ret = registeruserrepository.save(registeruser);
            return ret;
        }
        return null;
    }

    public Object deleteuser(String username){
        RegisterUser registeruser = registeruserrepository.findOneByName(username);
        if(null!=registeruser){
            registeruserrepository.deleteById(registeruser.getId());
            return registeruser;
        }
        return null;
    }

    public RegisterUser userlogout(String sessionid){
        RegisterUser findout = registeruserrepository.findOneBySessionid(sessionid);
        findout.setSessionkey(null);
        RegisterUser saveresult = registeruserrepository.save(findout);
        return saveresult;
    }

}
