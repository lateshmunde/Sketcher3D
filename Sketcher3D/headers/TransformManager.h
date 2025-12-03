#pragma once

#include <QMatrix4x4>
#include <vector>

// Keeps a transform matrix for each shape (same index as in ShapeManager)
class TransformManager
{
public:
    TransformManager() = default;

    // Add identity transform for a newly created shape
    void addIdentityTransform()
    {
        QMatrix4x4 m;
        m.setToIdentity();
        mTransforms.push_back(m);
    }

    // Remove transform at index (use when deleting a shape)
    void removeTransform(int index)
    {
        if (index >= 0 && index < static_cast<int>(mTransforms.size()))
            mTransforms.erase(mTransforms.begin() + index);
    }

    int count() const
    {
        return static_cast<int>(mTransforms.size());
    }

    // Non-const access (for translating / rotating / scaling)
    QMatrix4x4& transformAt(int index)
    {
        return mTransforms.at(index);   // throws if out of range ? good for debugging
    }

    // Const access (for rendering)
    const std::vector<QMatrix4x4>& getTransforms() const
    {
        return mTransforms;
    }

private:
    std::vector<QMatrix4x4> mTransforms;
};
