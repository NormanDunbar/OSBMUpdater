.mode column
.headers on
.nullvalue NULL

pragma foreign_keys= on;

create table Toolkit(
    tk_id integer primary key,
    tk_code varchar(10),
    tk_name varchar(50)
);

create unique index ix_tkcode on toolkit(tk_code);
create unique index ix_tkname on toolkit(tk_name);


create table keyword(
    kw_id integer primary key,
    kw_tkid integer,
    kw_name varchar(50),
    kw_description blob,
    foreign key (kw_tkid) references toolkit(tk_id)
);

create index ixfk_kwtkid on keyword (kw_tkid);
create index ix_kwname on keyword(kw_name);

create table syntax(
    sx_id integer primary key,
    sx_kwid integer,
    sx_syntax blob,
    foreign key (sx_kwid) references keyword(kw_id)
    on delete cascade
);

create index ixfk_sxkwid on syntax (sx_kwid);

create table example(
    ex_id integer primary key,
    ex_kwid integer,
    ex_name varchar(50),
    ex_example blob,
    foreign key (ex_kwid) references keyword(kw_id)
    on delete cascade
);

create index ixfk_exkwid on example (ex_kwid);

create table note(
    nt_id integer primary key,
    nt_kwid integer,
    nt_name varchar(50),
    nt_note blob,
    foreign key (nt_kwid) references keyword(kw_id)
    on delete cascade
);

create index ixfk_ntkwid on note (nt_kwid);

create table xref(
    xf_id integer primary key,
    xf_kwid integer,
    xf_name varchar(50),
    foreign key (xf_kwid) references keyword(kw_id)
    on delete cascade
);

create index ixfk_xfkwid on xref (xf_kwid);

