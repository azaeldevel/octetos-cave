
CREATE TABLE Progress (id BIGINT NOT NULL, stocking BIGINT NOT NULL, operation BIGINT NOT NULL, step SMALLINT NOT NULL DEFAULT 0 COMMENT 'Identificador de progreso actual');
ALTER TABLE Progress MODIFY id BIGINT NOT NULL AUTO_INCREMENT PRIMARY KEY;
-- ALTER TABLE Progress ADD CONSTRAINT pk_Progress PRIMARY KEY(stocking);
ALTER TABLE Progress ADD CONSTRAINT fk_stocking_Stocking_stocking FOREIGN KEY(stocking) REFERENCES Stocking(id);
ALTER TABLE Progress ADD CONSTRAINT fk_operation_Operation_operation FOREIGN KEY(operation) REFERENCES Operation(id);
-- ALTER TABLE Progress ADD updated datetime DEFAULT CURRENT_TIMESTAMP COMMENT 'Hora de actualizacion de progreso';


