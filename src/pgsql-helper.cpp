/**
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * This file is part of osm2pgsql (https://osm2pgsql.org/).
 *
 * Copyright (C) 2006-2026 by the osm2pgsql developer community.
 * For a full list of authors see the git log.
 */

#include "pgsql-helper.hpp"

#include "format.hpp"
#include "pgsql.hpp"
#include "pgsql-capabilities.hpp"

void analyze_table(pg_conn_t const &db_connection, std::string const &schema,
                   std::string const &name)
{
    auto const qual_name = qualified_name(schema, name);
    db_connection.exec("ANALYZE {}", qual_name);
}

void drop_table_if_exists(pg_conn_t const &db_connection,
                          std::string const &schema, std::string const &name)
{
    db_connection.exec("DROP TABLE IF EXISTS {} CASCADE",
                       qualified_name(schema, name));
}
