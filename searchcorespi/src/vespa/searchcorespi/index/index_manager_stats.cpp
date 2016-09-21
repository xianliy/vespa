// Copyright 2016 Yahoo Inc. Licensed under the terms of the Apache 2.0 license. See LICENSE in the project root.

#include <vespa/fastos/fastos.h>
#include "index_manager_stats.h"
#include "iindexmanager.h"
#include "indexsearchablevisitor.h"


namespace searchcorespi {

namespace {

class Visitor : public IndexSearchableVisitor
{
public:
    std::vector<index::DiskIndexStats> _diskIndexes;
    std::vector<index::MemoryIndexStats> _memoryIndexes;

    Visitor()
        : _diskIndexes(),
          _memoryIndexes()
    {
    }
    virtual void visit(const index::IDiskIndex &index) override {
        _diskIndexes.emplace_back(index);
    }
    virtual void visit(const index::IMemoryIndex &index) override {
        _memoryIndexes.emplace_back(index);
    }

    void normalize() {
        std::sort(_diskIndexes.begin(), _diskIndexes.end());
        std::sort(_memoryIndexes.begin(), _memoryIndexes.end());
    }
};

}

IndexManagerStats::IndexManagerStats()
    : _diskIndexes(),
      _memoryIndexes()
{
}

IndexManagerStats::IndexManagerStats(const IIndexManager &indexManager)
    : _diskIndexes(),
      _memoryIndexes()
{
    Visitor visitor;
    IndexSearchable::SP searchable(indexManager.getSearchable());
    searchable->accept(visitor);
    visitor.normalize();
    _diskIndexes = std::move(visitor._diskIndexes);
    _memoryIndexes = std::move(visitor._memoryIndexes);
}

IndexManagerStats::~IndexManagerStats()
{
}

} // namespace searchcorespi
