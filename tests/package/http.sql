
CREATE TABLE Session (id BIGINT, client VARCHAR(256) NOT NULL, session VARCHAR(128) COMMENT 'Identificador de la session');
ALTER TABLE Session MODIFY id BIGINT NOT NULL AUTO_INCREMENT PRIMARY KEY;
ALTER TABLE Session ADD CONSTRAINT  Session_session UNIQUE (session);

ALTER TABLE Variable ADD session BIGINT COMMENT 'La llave de referenciada http';
ALTER TABLE Variable ADD CONSTRAINT fk_Variable_session_Ente_id FOREIGN KEY(session) REFERENCES Session(id);
