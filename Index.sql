--Creacion de 3 tablas e ingreso de datos para probar indices
CREATE TABLE conductor (id_conductor SERIAL PRIMARY KEY, nombre VARCHAR(100) NOT NULL,
                        dni VARCHAR(8) UNIQUE NOT NULL, telefono VARCHAR(15));

CREATE TABLE vehiculo (id_vehiculo SERIAL PRIMARY KEY, placa VARCHAR(10) NOT NULL,
                       marca VARCHAR(50), modelo VARCHAR(50), anio INTEGER);

CREATE TABLE viaje (id_viaje SERIAL PRIMARY KEY, id_conductor INTEGER REFERENCES conductor(id_conductor),
                    id_vehiculo INTEGER REFERENCES vehiculo(id_vehiculo), fecha_inicio TIMESTAMP NOT NULL,
                    fecha_fin TIMESTAMP, origen VARCHAR(100), destino VARCHAR(100));

INSERT INTO conductor (nombre, dni, telefono)
VALUES
('Juan Pérez', '73458219', '987654321'),
('María López', '81234567', '956874321'),
('Carlos Ruiz', '76549821', '945612378'),
('Ana Torres', '70123456', '998741256'),
('Luis Mendoza', '78965412', '912345678');

INSERT INTO vehiculo (placa, marca, modelo, anio)
VALUES
('ABC-123', 'Toyota', 'Corolla', 2021),
('XYZ-456', 'Hyundai', 'Accent', 2020),
('LMN-789', 'Kia', 'Rio', 2022),
('DEF-321', 'Nissan', 'Versa', 2019),
('GHI-654', 'Chevrolet', 'Onix', 2023);

INSERT INTO viaje(id_conductor, id_vehiculo, fecha_inicio, fecha_fin, origen, destino)
VALUES
(1,1,'2026-07-01 08:00','2026-07-01 10:30','Piura','Sullana'),
(2,3,'2026-07-02 09:15','2026-07-02 12:00','Piura','Paita'),
(3,5,'2026-07-03 07:00','2026-07-03 09:45','Catacaos','Piura'),
(4,2,'2026-07-04 14:00','2026-07-04 17:20','Sullana','Talara'),
(5,4,'2026-07-05 06:45','2026-07-05 11:00','Piura','Chiclayo');

--Creacion de una nueva tabla para probar el indice gist
CREATE TABLE ubicacion (id_ubicacion SERIAL PRIMARY KEY, id_vehiculo INTEGER REFERENCES vehiculo(id_vehiculo),
                        posicion POINT NOT NULL, fecha_registro TIMESTAMP NOT NULL);

INSERT INTO ubicacion (id_vehiculo, posicion, fecha_registro)
VALUES
(1, POINT(-80.6328, -5.1945), '2026-07-01 08:15:00'),
(2, POINT(-80.6220, -5.1802), '2026-07-02 09:30:00'),
(3, POINT(-80.6854, -5.2667), '2026-07-03 07:45:00'),
(4, POINT(-81.2764, -4.5773), '2026-07-04 14:20:00'),
(5, POINT(-79.8449, -6.7714), '2026-07-05 10:10:00');

--Creacion de indice con BTREE
CREATE INDEX idx_viaje_fecha
ON viaje USING BTREE (fecha_inicio);

SELECT * FROM viaje
WHERE fecha_inicio BETWEEN '2026-07-01' AND'2026-07-03';

--Creacion de indice con un hash para encontrar una placa
CREATE INDEX idx_vehiculo_placa_hash
ON vehiculo USING HASH (placa);

SELECT * FROM vehiculo
WHERE placa = 'LMN-789';

--Creacion del indicie gist
CREATE INDEX idx_ubicacion_gist
ON ubicacion USING GIST (posicion);

SELECT * FROM ubicacion
WHERE posicion <@ box(point(-80.70,-5.30), point(-80.50,-5.10));