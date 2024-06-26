package com.sklois.SecureCloudStorageSystem.service;

import com.alibaba.fastjson.JSONArray;
import com.alibaba.fastjson.JSONObject;
import com.sklois.SecureCloudStorageSystem.entity.AuditEntity;
import com.sklois.SecureCloudStorageSystem.entity.Metadata;
import com.sklois.SecureCloudStorageSystem.entity.RegisterUser;
import com.sklois.SecureCloudStorageSystem.repository.MetadataRepository;
import com.sklois.SecureCloudStorageSystem.repository.RegisterUserRepository;

import jakarta.persistence.EntityManager;
import jakarta.persistence.Query;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.domain.*;
import org.springframework.stereotype.Service;


import java.math.BigInteger;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

@Service
public class MetadataSerImpl implements MetadataSer {

    @Autowired
    private EntityManager entityManager;

    @Autowired
    private MetadataRepository metadatarepository;

    @Autowired
    DockingSecurityCloudSer dockingsecuritycloudser;


    @Autowired
    private RegisterUserRepository registeruserrepository;

    public Metadata savemetadata(String metadata) {

        JSONObject jsonObject = JSONObject.parseObject(metadata);

        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");

        String filename = jsonObject.getString("filename");
        long filesize = jsonObject.getLong("filesize");
        LocalDateTime ctime = LocalDateTime.parse(jsonObject.getString("ctime"), formatter);
        LocalDateTime mtime = LocalDateTime.parse(jsonObject.getString("mtime"), formatter);
        LocalDateTime lastaccesstime = LocalDateTime.parse(jsonObject.getString("lastaccesstime"), formatter);
        LocalDateTime uploadtime = LocalDateTime.parse(jsonObject.getString("uploadtime"), formatter);
        String fileowner = jsonObject.getString("fileowner");
        String plaintexthash = jsonObject.getString("plaintexthash");
        String ciphertexthash = jsonObject.getString("ciphertexthash");
        long ciphertextsize = jsonObject.getLong("ciphertextsize");
        String keyhash = jsonObject.getString("keyhash");
        String fileuniqueid = jsonObject.getString("fileuniqueid");

        String type = jsonObject.getString("type");

        String currentid = jsonObject.getString("currentid");

        String parentid = jsonObject.getString("parentid");


        Metadata findout = metadatarepository.findOneByFileuniqueid(fileuniqueid);
        if (null == findout) {
            Metadata metadataobj = new Metadata();
            metadataobj.setFilename(filename);
            metadataobj.setFilesize(filesize);
            metadataobj.setCtime(ctime);
            metadataobj.setMtime(mtime);
            metadataobj.setLastaccesstime(lastaccesstime);
            metadataobj.setUploadtime(uploadtime);
            metadataobj.setFileowner(fileowner);
            metadataobj.setPlaintexthash(plaintexthash);
            metadataobj.setCiphertexthash(ciphertexthash);
            metadataobj.setCiphertextsize(ciphertextsize);
            metadataobj.setKeyhash(keyhash);
            metadataobj.setFileuniqueid(fileuniqueid);
            metadataobj.setType(type);
            metadataobj.setCurrentid(currentid);
            metadataobj.setParentid(parentid);
            Metadata saveresult = metadatarepository.save(metadataobj);
            return saveresult;
        } else {
            return null;
        }


    }

    public List<JSONObject> folderlist(String username) {
        List<JSONObject> ret = get_folder_list("根目录", username);
        return ret;
    }

    List<JSONObject> get_folder_list(String parentid, String username) {

        List<JSONObject> listItems = new ArrayList<>();
        List<Metadata> result = metadatarepository.findAllByTypeAndParentidAndFileowner("dir", parentid, username);//
        if (null != result && result.size() > 0) {
            for (Metadata one : result) {
                if (0 == "dir".compareToIgnoreCase(one.getType())) {
                    JSONObject listItem = new JSONObject(true);
                    listItem.put("id", one.getId());
                    listItem.put("currentid", one.getCurrentid());
                    listItem.put("opened", one.isIfopened());
                    listItem.put("foldername", one.getFilename());
                    listItem.put("ctime", one.getCtime());
                    listItem.put("mtime", one.getMtime());
                    listItem.put("lastaccesstime", one.getLastaccesstime());
                    listItem.put("uploadtime", one.getUploadtime());
                    List<JSONObject> listItem_recursive = get_folder_list(one.getCurrentid(), username);
                    if (null != listItem_recursive && listItem_recursive.size() > 0) {
                        listItem.put("children", listItem_recursive);
                    }
                    listItems.add(listItem);
                }
            }
        }
        return listItems;
    }

    public List<JSONObject> filelist(
            int pageNo,
            int pageSize,
            String folderuniqueid,
            String username) {
        Page<Metadata> findoutpage = null;
        Sort sort = Sort.by(Sort.Direction.DESC, "id");
        Pageable pageable = PageRequest.of(pageNo, pageSize, sort);
        ExampleMatcher matcher = ExampleMatcher.matching()
                .withStringMatcher(ExampleMatcher.StringMatcher.EXACT)  //改变默认字符串匹配为:模糊查询
                .withMatcher("parentid", ExampleMatcher.GenericPropertyMatchers.exact())
                .withMatcher("fileowner", ExampleMatcher.GenericPropertyMatchers.exact())
                .withIgnorePaths("id")
                .withIgnorePaths("ciphertextsize")
                .withIgnorePaths("currentid")
                .withIgnorePaths("filename")
                .withIgnorePaths("filesize")
                .withIgnorePaths("fileuniqueid")
                .withIgnorePaths("ifopened")
                .withIgnorePaths("ifshared"); //忽略id字段
        System.out.println("username is " + username);
        Metadata filemetadata = new Metadata();
        filemetadata.setParentid(folderuniqueid);
        filemetadata.setFileowner(username);
        Example<Metadata> ex = Example.of(filemetadata, matcher); //动态查询
        findoutpage = metadatarepository.findAll(ex, pageable);

        List<Metadata> listout = findoutpage.getContent();
        System.out.println("listout size is " + listout.size());
        List<JSONObject> listItems = new ArrayList<>();
        if (null != listout && listout.size() > 0) {
            for (Metadata one : listout) {
                JSONObject listItem = new JSONObject(true);
                listItem.put("id", one.getId());
                listItem.put("filename", one.getFilename());
                listItem.put("filesize", one.getFilesize());
                listItem.put("ctime", one.getCtime());
                listItem.put("mtime", one.getMtime());
                listItem.put("lastaccesstime", one.getLastaccesstime());
                listItem.put("uploadtime", one.getUploadtime());
                listItem.put("plaintexthash", one.getPlaintexthash());
                listItem.put("ciphertexthash", one.getCiphertexthash());
                listItem.put("ciphertextsize", one.getCiphertextsize());
                listItem.put("keyhash", one.getKeyhash());
                listItem.put("fileuniqueid", one.getFileuniqueid());
                listItem.put("type", one.getType());
                listItem.put("currentid", one.getCurrentid());
                listItem.put("parentid", one.getParentid());
                listItem.put("ifshared", one.isIfshared());
                listItem.put("opened", one.isIfopened());
                listItems.add(listItem);
            }
        }
        return listItems;
    }

    public long countByParentidAndFileowner(String parentid, String fileowner) {
        long filenumber = metadatarepository.countByParentidAndFileowner(parentid, fileowner);
        return filenumber;
    }


    public List<Metadata> datasearch(
            String sessionid,
            String mode,
            String condition,
            JSONArray keywords,
            String type,
            String fromtime,
            String totime,
            int pageNo,
            int pageSize) {
        RegisterUser fuser = registeruserrepository.findOneBySessionid(sessionid);

        if (0 == mode.compareToIgnoreCase("filename")) {
            StringBuilder sqlStr = new StringBuilder("select * from metadata where ");
            Map<String, Object> parameters = new HashMap<>();
            int index = 0;
            sqlStr.append("(");
            for (int i = 0; i < keywords.size(); i++) {
                if (i > 0) {
                    sqlStr.append(null != condition && 0 == condition.compareToIgnoreCase("or") ? " or " : " and ");
                }
                sqlStr.append("filename like :keyword" + index);
                parameters.put("keyword" + index, "%" + keywords.get(i) + "%");
                index++;
            }
            sqlStr.append(")");

            sqlStr.append(" and (fileowner = :fileowner)");
            parameters.put("fileowner", fuser.getName());

            String countQueryStr = "SELECT count(*) " + sqlStr.substring(sqlStr.indexOf("from"));
            System.out.println("sql is " + sqlStr.toString());
            System.out.println("countSql is " + countQueryStr);

            Query query = entityManager.createNativeQuery(sqlStr.toString(), Metadata.class);
            Query countQuery = entityManager.createNativeQuery(countQueryStr);
            for (Map.Entry<String, Object> entry : parameters.entrySet()) {
                query.setParameter(entry.getKey(), entry.getValue());
                countQuery.setParameter(entry.getKey(), entry.getValue());
            }

            // Handle count result as Long or BigInteger
            Object countResult = countQuery.getSingleResult();
            long total = (countResult instanceof BigInteger) ? ((BigInteger) countResult).longValue() : (Long) countResult;
            System.out.println("total is " + total);
            query.setFirstResult(pageNo * pageSize);
            query.setMaxResults(pageSize);

            return query.getResultList();
        } else if (0 == mode.compareToIgnoreCase("cipherindex")) {
            String retstr = dockingsecuritycloudser.searchcipherindex("AND", keywords);
            System.out.println("retstr is " + retstr);

            JSONObject jsonObject = JSONObject.parseObject(retstr);
            JSONObject details = jsonObject.getJSONObject("details");
            JSONArray searchresult = details.getJSONArray("searchresult");

            if (searchresult.size() > 0) {
                StringBuilder sqlStr = new StringBuilder("select * from metadata where ");
                Map<String, Object> parameters = new HashMap<>();
                int index = 0;
                sqlStr.append("(");
                for (int i = 0; i < searchresult.size(); i++) {
                    if (i > 0) {
                        sqlStr.append(" or ");
                    }
                    sqlStr.append("fileuniqueid = :fileuniqueid" + index);
                    parameters.put("fileuniqueid" + index, searchresult.getJSONObject(i).getString("fileuniqueid"));
                    index++;
                }
                sqlStr.append(")");

                if (null != type && !type.equalsIgnoreCase("") && !type.equalsIgnoreCase("*")) {
                    sqlStr.append(" and (");
                    boolean firstType = true;
                    if (type.contains("p")) {
                        if (!firstType) sqlStr.append(" or ");
                        sqlStr.append("filename like :typePng or filename like :typeJpg or filename like :typeJpeg");
                        parameters.put("typePng", "%.png");
                        parameters.put("typeJpg", "%.jpg");
                        parameters.put("typeJpeg", "%.jpeg");
                        firstType = false;
                    }
                    if (type.contains("d")) {
                        if (!firstType) sqlStr.append(" or ");
                        sqlStr.append("filename like :typeDoc");
                        parameters.put("typeDoc", "%.doc");
                        firstType = false;
                    }
                    if (type.contains("v")) {
                        if (!firstType) sqlStr.append(" or ");
                        sqlStr.append("filename like :typeMp4");
                        parameters.put("typeMp4", "%.mp4");
                        firstType = false;
                    }
                    if (type.contains("m")) {
                        if (!firstType) sqlStr.append(" or ");
                        sqlStr.append("filename like :typeMp3");
                        parameters.put("typeMp3", "%.mp3");
                    }
                    sqlStr.append(")");
                }

                if (null != fromtime && !fromtime.equalsIgnoreCase("") &&
                        null != totime && !totime.equalsIgnoreCase("")) {
                    sqlStr.append(" and (uploadtime between :fromtime and :totime)");
                    DateTimeFormatter df = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
                    parameters.put("fromtime", LocalDateTime.parse(fromtime, df));
                    parameters.put("totime", LocalDateTime.parse(totime, df));
                }

                sqlStr.append(" and (fileowner = :fileowner)");
                parameters.put("fileowner", fuser.getName());

                String countQueryStr = "SELECT count(*) " + sqlStr.substring(sqlStr.indexOf("from"));
                System.out.println("sql is " + sqlStr.toString());
                System.out.println("countSql is " + countQueryStr);

                Query query = entityManager.createNativeQuery(sqlStr.toString(), Metadata.class);
                Query countQuery = entityManager.createNativeQuery(countQueryStr);
                for (Map.Entry<String, Object> entry : parameters.entrySet()) {
                    query.setParameter(entry.getKey(), entry.getValue());
                    countQuery.setParameter(entry.getKey(), entry.getValue());
                }

                // Handle count result as Long or BigInteger
                Object countResult = countQuery.getSingleResult();
                long total = (countResult instanceof BigInteger) ? ((BigInteger) countResult).longValue() : (Long) countResult;
                System.out.println("total is " + total);
                query.setFirstResult(pageNo * pageSize);
                query.setMaxResults(pageSize);

                return query.getResultList();
            }
        }
        return null;
    }



    public boolean checkauth(String fileuniqueid,String sessionid){
        RegisterUser fuser = registeruserrepository.findOneBySessionid(sessionid); //根据sessionid查询用户标识信息
        Metadata ret= metadatarepository.findOneByFileuniqueidAndFileowner(fileuniqueid,fuser.getName());//根据文件唯一id和用户查询元信息
        if(null!=ret){//如果查询结果不为空，则返回true
            return true;
        }else{
            return false;
        }
    }

}
