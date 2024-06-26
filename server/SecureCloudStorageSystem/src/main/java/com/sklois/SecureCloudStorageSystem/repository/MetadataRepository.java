package com.sklois.SecureCloudStorageSystem.repository;

import com.sklois.SecureCloudStorageSystem.entity.Metadata;
import com.sklois.SecureCloudStorageSystem.entity.RegisterUser;
import jakarta.transaction.Transactional;
import org.springframework.data.jpa.repository.JpaRepository;

import java.util.List;

public interface MetadataRepository extends JpaRepository<Metadata, Integer> {

    public  Metadata findOneByFileuniqueid(String fileuniqueid);
    public  Metadata findOneByFileuniqueidAndFileowner(String fileuniqueid,String fileowner);

    public List<Metadata> findAllByTypeAndParentidAndFileowner(
            String type,String parentid,String fileowner);
    public List<Metadata> findAllByParentidAndFileowner(
            String parentid,String fileowner);
    long countByParentidAndFileowner(String parentid,String fileowner);


    public  Metadata findOneByIdAndFileowner(Integer id, String fileowner);
    @Transactional
    void deleteByFileuniqueid(String fileuniqeid);
}
