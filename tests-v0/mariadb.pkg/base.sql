

CREATE TABLE Version (id BIGINT NOT NULL, name VARCHAR(20) UNIQUE, major SMALLINT NOT NULL,minor SMALLINT,patch SMALLINT); 
ALTER TABLE Version MODIFY id BIGINT NOT NULL AUTO_INCREMENT PRIMARY KEY;

CREATE TABLE Person (id BIGINT, name1 VARCHAR(30) NOT NULL,name2 VARCHAR(30),name3 VARCHAR(30),name4 VARCHAR(30));
ALTER TABLE Person MODIFY id BIGINT NOT NULL AUTO_INCREMENT PRIMARY KEY;
-- ALTER TABLE Person ADD CONSTRAINT fk_Person_ente_Ente_id FOREIGN KEY(ente) REFERENCES Ente(id);
-- ALTER TABLE Person ADD CONSTRAINT ente_unique UNIQUE (ente);
ALTER TABLE Person ADD COLUMN age DECIMAL(6,2);
ALTER TABLE Person ADD COLUMN canyonNumber DECIMAL(6,2);
ALTER TABLE Person ADD COLUMN gender ENUM('M','F');
ALTER TABLE Person ADD COLUMN canyonLength DECIMAL(6,2);
ALTER TABLE Person MODIFY COLUMN age FLOAT;
ALTER TABLE Person MODIFY COLUMN canyonNumber FLOAT;
ALTER TABLE Person MODIFY COLUMN canyonLength FLOAT;
-- ALTER TABLE Person ADD CONSTRAINT ente_primary PRIMARY KEY (ente);
ALTER TABLE Person MODIFY COLUMN canyonLength FLOAT;

CREATE TABLE UserManagement(id BIGINT NOT NULL);
ALTER TABLE UserManagement MODIFY id BIGINT NOT NULL AUTO_INCREMENT PRIMARY KEY;

CREATE TABLE User (id BIGINT NOT NULL,person BIGINT NOT NULL, name VARCHAR(20) NOT NULL,pwdtxt VARCHAR(12));
ALTER TABLE User MODIFY id BIGINT NOT NULL PRIMARY KEY;
ALTER TABLE User ADD CONSTRAINT fk_id_UserManagement_id FOREIGN KEY(id) REFERENCES UserManagement(id);
ALTER TABLE User ADD CONSTRAINT fk_person_Person_ente FOREIGN KEY(person) REFERENCES Person(id);
ALTER TABLE User ADD CONSTRAINT users_unique UNIQUE (name);
ALTER TABLE User ADD COLUMN status SMALLINT;

CREATE TABLE Operation(id BIGINT NOT NULL);
ALTER TABLE Operation MODIFY id BIGINT NOT NULL AUTO_INCREMENT PRIMARY KEY;

CREATE TABLE Variable (id BIGINT NOT NULL, name VARCHAR(32), value VARCHAR(64)); 
ALTER TABLE Variable MODIFY id BIGINT NOT NULL AUTO_INCREMENT PRIMARY KEY;
