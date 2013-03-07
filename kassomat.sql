drop database if exists kassomat;
create database kassomat with encoding='UTF8';
\c kassomat

create table users (
    id bigserial primary key,
    email varchar(100) not null
);

create table mos_users (
    mos_id varchar(100) not null,
    ibutton_id varchar(20) null
) inherits(users);

create table admins (
    password varchar(100) not null
) inherits(mos_users);


create table moneycodes (
    id bigserial primary key,
    user bigint references users,
    code varchar(100) not null
);

create table projects (
    id bigserial primary key,
    name varchar(100) not null,
    description varchar(2000) null,
    valid_until date null,
    funding_target_min integer null,
    funding_target_max integer null,
    admin_password varchar(100) not null,
    notification_email varchar(100) null,
    sort_order integer not null
);

create table products (
    id bigserial primary key,
    project bigint references projects,
    display_name varchar(200) not null,
--  foreign_id: cliffords meta data damit er seine bauteile matchen kann.
    foreign_id varchar(200) not null,
    barcode varchar(20) null,
    cost integer not null
);

-- a booking consists of some metadata and one or more booking_rows
create table bookings (
    id bigserial primary key,
    booking_time timestamp default localtimestamp,
    admin_id bigint null references admins,
    user_email varchar(200) null
);

-- booking row that contains description and amount for a booking as well project id
-- and info to identify the product if the booking is not specifically for the projects
create table booking_rows (
    id bigserial primary key,
    booking bigint not null references bookings on delete cascade,
    text varchar(200) not null,
    product_foreign_id varchar(200) null,
    cost_per_item integer not null,
    amount integer not null,
    project bigint not null references projects
);


