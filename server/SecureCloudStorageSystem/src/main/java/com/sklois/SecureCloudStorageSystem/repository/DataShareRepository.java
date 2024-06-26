package com.sklois.SecureCloudStorageSystem.repository;

import com.sklois.SecureCloudStorageSystem.entity.DataShareEntity;
import com.sklois.SecureCloudStorageSystem.entity.Metadata;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.Pageable;
import org.springframework.data.jpa.repository.JpaRepository;

import java.util.List;

public interface DataShareRepository extends JpaRepository<DataShareEntity, Integer> {
    public List<DataShareEntity> findAllByFromid(Integer fromid);
    public List<DataShareEntity> findAllByFileid(Integer fileid);
    public List<DataShareEntity> findAllByToid(Integer toid);
    public DataShareEntity findOneByFromidAndFileidAndToid(Integer fromid,Integer fileid ,Integer toid);
    public DataShareEntity findOneByFileidAndToid(Integer fileid, Integer toid);
}
