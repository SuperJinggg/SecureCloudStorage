package com.sklois.SecureCloudStorageSystem.service;

import com.alibaba.fastjson.JSONObject;
import com.sklois.SecureCloudStorageSystem.entity.DataShareEntity;
import com.sklois.SecureCloudStorageSystem.entity.Metadata;
import com.sklois.SecureCloudStorageSystem.entity.RegisterUser;
import com.sklois.SecureCloudStorageSystem.repository.DataShareRepository;
import com.sklois.SecureCloudStorageSystem.repository.MetadataRepository;
import com.sklois.SecureCloudStorageSystem.repository.RegisterUserRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;


@Service
public class DataShareSerImpl implements DataShareSer {

    @Autowired
    private RegisterUserRepository registeruserrepository;

    @Autowired
    private MetadataRepository metadatadao;

    @Autowired
    private DataShareRepository datasharedao;
    @Autowired
    private DataShareRepository datasharerepository;
    @Override
    public HashMap<String, Object> sharedata(
            String sessionid,
            String fileuniqueid,
            String shareusername) {

        HashMap<String, Object> retmap = new HashMap<String, Object>();
        RegisterUser fuser = registeruserrepository.findOneBySessionid(sessionid);
        if (null != fuser) {
            Metadata metadata = metadatadao.findOneByFileuniqueidAndFileowner(fileuniqueid, fuser.getName());
            if (null != metadata) {
                RegisterUser tuser = registeruserrepository.findOneByName(shareusername);
                if (null != tuser) {

                    if (0 != fuser.getName().compareToIgnoreCase(tuser.getName())) {
                        DataShareEntity datashareentity = new DataShareEntity();
                        datashareentity.setFromid(fuser.getId());
                        datashareentity.setFileid(metadata.getId());
                        datashareentity.setToid(tuser.getId());
                        LocalDateTime currentTime = LocalDateTime.now(); // get the current date and time
                        datashareentity.setSharetime(currentTime);
                        //先查询是否有这个分享
                        DataShareEntity findout = datasharedao.findOneByFromidAndFileidAndToid(
                                fuser.getId(),
                                metadata.getId(),
                                tuser.getId()
                        );
                        if (null == findout) {
                            DataShareEntity saveret = datasharedao.save(datashareentity);
                            if (null != saveret) {
                                retmap.put("result", true);
                                retmap.put("message", "share success");
                                retmap.put("saveret", saveret);
                                metadata.setIfshared(true);
                                metadatadao.save(metadata);
                                return retmap;
                            } else {
                                retmap.put("result", false);
                                retmap.put("message", "save error");
                                return retmap;
                            }
                        } else {
                            retmap.put("result", false);
                            retmap.put("message", "this file have already shared");
                            return retmap;
                        }
                    } else {
                        retmap.put("result", false);
                        retmap.put("message", "username are same");
                        return retmap;
                    }

                } else {
                    retmap.put("result", false);
                    retmap.put("message", "share to username not exist");
                    return retmap;
                }
            } else {
                retmap.put("result", false);
                retmap.put("message", "share file not found");
                return retmap;
            }
        } else {
            retmap.put("result", false);
            retmap.put("message", "share from username error");
            return retmap;
        }
    }

    public HashMap<String, Object> getsharelist(String sessionid) {
        HashMap<String, Object> retmap = new HashMap<String, Object>();
        RegisterUser fuser = registeruserrepository.findOneBySessionid(sessionid);
        List<DataShareEntity> findout = datasharedao.findAllByFromid(fuser.getId());

        if(null != findout && findout.size() > 0){
            int filenumber = findout.size();
            List<JSONObject> listItems = new ArrayList<>();
            for (DataShareEntity one : findout) {
                JSONObject listItem = new JSONObject(true);
                RegisterUser tuser = registeruserrepository.findOneById(one.getToid());

                Metadata metadata = metadatadao.findOneByIdAndFileowner(one.getFileid(),fuser.getName());

                if(null!=metadata){
                    listItem.put("sharedusername", tuser.getName());
                    DateTimeFormatter df = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
                    listItem.put("sharetime", df.format(one.getSharetime()));
                    listItem.put("filename", metadata.getFilename());
                    listItem.put("filesize", metadata.getFilesize());
                    listItem.put("ctime", metadata.getCtime());
                    listItem.put("mtime", metadata.getMtime());
                    listItem.put("lastaccesstime", metadata.getLastaccesstime());
                    listItem.put("uploadtime", metadata.getUploadtime());
                    listItem.put("plaintexthash", metadata.getPlaintexthash());
                    listItem.put("ciphertexthash", metadata.getCiphertexthash());
                    listItem.put("ciphertextsize", metadata.getCiphertextsize());
                    listItem.put("keyhash", metadata.getKeyhash());
                    listItem.put("fileuniqueid", metadata.getFileuniqueid());
                    listItem.put("type", metadata.getType());
                    listItem.put("currentid", metadata.getCurrentid());
                    listItem.put("parentid", metadata.getParentid());
                    listItem.put("ifshared", metadata.isIfshared());
                    listItem.put("opened", metadata.isIfopened());
                    listItems.add(listItem);
                }

            }
            retmap.put("result", true);
            retmap.put("message", "getsharelist success");
            retmap.put("filenumber", filenumber);
            retmap.put("filelist", listItems);
            return retmap;

        }else{
            retmap.put("result", false);
            retmap.put("message", "share file not found");
            return retmap;
        }
    }

    public HashMap<String, Object> cancelshare(
            String sessionid,
            String fileuniqueid,
            String sharedusername) {
        HashMap<String, Object> retmap = new HashMap<String, Object>();
        RegisterUser fuser = registeruserrepository.findOneBySessionid(sessionid);
        RegisterUser tuser = registeruserrepository.findOneByName(sharedusername);
        Metadata metadata = metadatadao.findOneByFileuniqueidAndFileowner(fileuniqueid, fuser.getName());
        DataShareEntity findout =
                datasharedao.findOneByFromidAndFileidAndToid(
                        fuser.getId(),
                        metadata.getId(),
                        tuser.getId());
        if(null!=findout){
            datasharedao.deleteById(findout.getId());
        }
        return retmap;
    }

    public HashMap<String, Object> getsharedlist(String sessionid) {
        HashMap<String, Object> retmap = new HashMap<String, Object>();
        RegisterUser tuser = registeruserrepository.findOneBySessionid(sessionid);
        List<DataShareEntity> findout = datasharedao.findAllByToid(tuser.getId());
        if(null != findout && findout.size() > 0){
            int filenumber = findout.size();
            List<JSONObject> listItems = new ArrayList<>();
            for (DataShareEntity one : findout) {
                JSONObject listItem = new JSONObject(true);
                RegisterUser fuser = registeruserrepository.findOneById(one.getFromid());
                Metadata metadata = metadatadao.findOneByIdAndFileowner(one.getFileid(),fuser.getName());
                if(null!=metadata){
                    listItem.put("shareusername", fuser.getName());
                    DateTimeFormatter df = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
                    listItem.put("sharetime", df.format(one.getSharetime()));
                    listItem.put("filename", metadata.getFilename());
                    listItem.put("filesize", metadata.getFilesize());
                    listItem.put("ctime", metadata.getCtime());
                    listItem.put("mtime", metadata.getMtime());
                    listItem.put("lastaccesstime", metadata.getLastaccesstime());
                    listItem.put("uploadtime", metadata.getUploadtime());
                    listItem.put("plaintexthash", metadata.getPlaintexthash());
                    listItem.put("ciphertexthash", metadata.getCiphertexthash());
                    listItem.put("ciphertextsize", metadata.getCiphertextsize());
                    listItem.put("keyhash", metadata.getKeyhash());
                    listItem.put("fileuniqueid", metadata.getFileuniqueid());
                    listItem.put("type", metadata.getType());
                    listItem.put("currentid", metadata.getCurrentid());
                    listItem.put("parentid", metadata.getParentid());
                    listItem.put("ifshared", metadata.isIfshared());
                    listItem.put("opened", metadata.isIfopened());
                    listItems.add(listItem);
                }

            }
            retmap.put("result", true);
            retmap.put("message", "getsharedlist success");
            retmap.put("filenumber", filenumber);
            retmap.put("filelist", listItems);
            return retmap;

        }else{
            retmap.put("result", false);
            retmap.put("message", "share file not found");
            return retmap;
        }
    }
    public boolean checkauth(String fileuniqueid,String sessionid){
        Metadata metadataentity = metadatadao.findOneByFileuniqueid(fileuniqueid);
        RegisterUser tuser = registeruserrepository.findOneBySessionid(sessionid);
        DataShareEntity shareentity  = datasharerepository.findOneByFileidAndToid(metadataentity.getId(),tuser.getId());
        if(null!=shareentity){
            return true;
        }else{
            return false;
        }
    }
}
