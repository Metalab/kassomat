--DROP DATABASE IF EXISTS kassomat;
--CREATE DATABASE kassomat WITH ENCODING='UTF8';

BEGIN;

CREATE TABLE users (
    id serial PRIMARY KEY,
    email varchar(100) NOT NULL
);

CREATE TABLE mos_users (
    mos_id varchar(100) NOT NULL,
    ibutton_id varchar(20) NULL,
    CONSTRAINT mos_users_pkey PRIMARY KEY (id)
) inherits(users);

CREATE TABLE admins (
    password varchar(100) NOT NULL,
    CONSTRAINT admins_pkey PRIMARY KEY (id)
) inherits(mos_users);

CREATE TABLE moneycodes (
    id serial PRIMARY KEY,
    user_id integer NOT NULL REFERENCES users (id),
    booking_id integer NOT NULL REFERENCES bookings (id),
    code varchar(100) NOT NULL
);

CREATE TABLE projects (
    id serial PRIMARY KEY,
    name varchar(100) NOT NULL,
    description text NULL,
    valid_until date NULL,
    funding_target_min integer NULL,
    funding_target_max integer NULL,
    admin_password varchar(100) NOT NULL,
    notification_email varchar(100) NULL,
    sort_order integer NOT NULL
);

CREATE TABLE products (
    id serial PRIMARY KEY,
    project_id integer NOT NULL REFERENCES projects(id),
    display_name varchar(200) NOT NULL,
--  foreign_id: cliffords meta data damit er seine bauteile matchen kann.
    foreign_id varchar(200) NULL,
    barcode varchar(20) NULL,
    cost integer NOT NULL
);

-- a booking consists of some metadata and one or more booking_rows
CREATE TABLE bookings (
    id serial PRIMARY KEY,
    booking_time timestamp default localtimestamp,
    admin_id integer NULL REFERENCES admins (id),
    user_email varchar(200) NULL
);

-- booking row that contains description and amount for a booking as well project id
-- and info to identify the product if the booking is not specifically for the projects
CREATE TABLE booking_rows (
    id serial PRIMARY KEY,
    booking_id integer NOT NULL REFERENCES bookings (id) ON DELETE CASCADE,
    text varchar(200) NOT NULL,
--  a copy of products (foreign_id)
    product_foreign_id varchar(200) NULL,
    cost_per_item integer NOT NULL,
    amount integer NOT NULL,
    project_id integer NOT NULL REFERENCES projects (id)
);

COMMIT;
