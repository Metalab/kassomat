create table admins (
    id bigserial primary key,
    name nvarchar(100) not null,
    password nvarchar(100) not null
)

create table projects (
    id bigserial primary key,
    name nvarchar(100) not null,
    description nvarchar(2000) null,
    valid_until date null,
    funding_target_min integer null,
    funding_target_max integer null,
    admin_password nvarchar(100) not null,
    notification_email nvarchar(100) null,
    sort_order integer not null
)

create table products (
    id bigserial primary key,
    project bigint references projects,
    display_name nvarchar(200) not null,
    foreign_id nvarchar(200) not,
    barcode nvarchar(20) null,
    cost integer not null
)

-- a booking consists of some metadata and one or more booking_rows
create table bookings (
    id bigserial primary key,
    when timestamp default localtimestamp,
    admin_id bigint null references admins,
    user_email nvarchar(200) null
)

-- booking row that contains description and amount for a booking as well project id
-- and info to identify the product if the booking is not specifically for the projects
create table booking_rows (
    id bigserial primary key,
    booking bigint not null references bookings on delete cascade,
    text nvarchar(200) not null,
    product_foreign_id nvarchar(200) null,
    amount integer not null,
    project bigint not null references projects
)


