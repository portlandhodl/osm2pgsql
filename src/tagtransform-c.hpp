#ifndef OSM2PGSQL_TAGTRANSFORM_C_HPP
#define OSM2PGSQL_TAGTRANSFORM_C_HPP

/**
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * This file is part of osm2pgsql (https://osm2pgsql.org/).
 *
 * Copyright (C) 2006-2026 by the osm2pgsql developer community.
 * For a full list of authors see the git log.
 */

#include "taginfo-impl.hpp"
#include "tagtransform.hpp"

#include <cstddef>
#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

class c_tagtransform_t : public tagtransform_t
{
public:
    c_tagtransform_t(options_t const *options, export_list_t exlist);

    std::unique_ptr<tagtransform_t> clone() const override;

    bool filter_tags(osmium::OSMObject const &o, bool *polygon, bool *roads,
                     taglist_t *out_tags) override;

    bool filter_rel_member_tags(taglist_t const &rel_tags,
                                osmium::memory::Buffer const &members,
                                rolelist_t const &member_roles,
                                bool *make_boundary, bool *make_polygon,
                                bool *roads, taglist_t *out_tags) override;

private:
    /**
     * Transparent string hash so that an unordered_map with std::string
     * keys can be searched with a char const * key without creating a
     * temporary std::string.
     */
    struct string_hash_t
    {
        using is_transparent = void;

        std::size_t operator()(std::string_view sv) const noexcept
        {
            return std::hash<std::string_view>{}(sv);
        }
    };

    /**
     * Preprocessed form of the export list of one object type, built
     * once in the constructor so that check_key() does not have to scan
     * the whole export list for each tag.
     */
    struct key_lookup_t
    {
        struct exact_entry_t
        {
            std::size_t pos;
            unsigned int flags;
        };

        struct wildcard_entry_t
        {
            std::string pattern;
            std::size_t pos;
        };

        // Entries without FLAG_DELETE never contain wildcards (this is
        // checked when the style file is read), so they match exactly.
        // Because emplace() keeps the first value for a key, this maps
        // the key to the entry with the smallest position, like the
        // sequential scan over the export list does.
        std::unordered_map<std::string, exact_entry_t, string_hash_t,
                           std::equal_to<>>
            exact;

        // All entries with FLAG_DELETE in file order. These are matched
        // with wild_match(), even if they don't contain wildcards.
        std::vector<wildcard_entry_t> wild;
    };

    bool check_key(osmium::item_type export_type, char const *k, bool *filter,
                   unsigned int *flags);

    options_t const *m_options;
    export_list_t m_export_list;
    osmium::nwr_array<key_lookup_t> m_key_lookup;
};

#endif // OSM2PGSQL_TAGTRANSFORM_C_HPP
