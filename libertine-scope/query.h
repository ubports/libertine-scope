/*
 * Copyright 2015-2016 Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3, as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LIBERTINE_SCOPE_QUERY_H_
#define LIBERTINE_SCOPE_QUERY_H_

#include "libertine-scope/libertine.h"
#include <unity/scopes/ReplyProxyFwd.h>
#include <unity/scopes/SearchQueryBase.h>

class QStringList;


/**
 * Engine to run a specific scope query.
 */
class Query
: public unity::scopes::SearchQueryBase
{
public:
    Query(unity::scopes::CannedQuery const&    query,
          unity::scopes::SearchMetadata const& metadata,
          Libertine::Factory const&            libertine_factory);

    ~Query() = default;

    void
    cancelled() override;

    void
    run(unity::scopes::SearchReplyProxy const& reply) override;

    // Overriding base class method to add ability to test
    virtual unity::scopes::VariantMap settings() const;

private:
    QStringList blacklist() const;

    Libertine::Factory libertine_factory_;
};

#endif // LIBERTINE_SCOPE_QUERY_H_


