/*
 * Copyright 2016 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrAppliedClip_DEFINED
#define GrAppliedClip_DEFINED

#include "GrFragmentProcessor.h"
#include "GrScissorState.h"
#include "GrWindowRectsState.h"

/**
 * Produced by GrClip. It provides a set of modifications to the drawing state that are used to
 * create the final GrPipeline for a GrOp.
 */
class GrAppliedClip : public SkNoncopyable {
public:
    const GrScissorState& scissorState() const { return fScissorState; }
    const GrWindowRectsState& windowRectsState() const { return fWindowRectsState; }
    GrFragmentProcessor* clipCoverageFragmentProcessor() const { return fClipCoverageFP.get(); }
    bool hasStencilClip() const { return fHasStencilClip; }

    /**
     * Intersects the applied clip with the provided rect. Returns false if the draw became empty.
     * 'clippedDrawBounds' will be intersected with 'irect'. This returns false if the clip becomes
     * empty or the draw no longer intersects the clip. In either case the draw can be skipped.
     */
    bool addScissor(const SkIRect& irect, SkRect* clippedDrawBounds) {
        return fScissorState.intersect(irect) && clippedDrawBounds->intersect(SkRect::Make(irect));
    }

    void addWindowRectangles(const GrWindowRectsState& windowState) {
        SkASSERT(!fWindowRectsState.enabled());
        fWindowRectsState = windowState;
    }

    void addWindowRectangles(const GrWindowRectangles& windows, GrWindowRectsState::Mode mode) {
        SkASSERT(!fWindowRectsState.enabled());
        fWindowRectsState.set(windows, mode);
    }

    void addCoverageFP(sk_sp<GrFragmentProcessor> fp) {
        SkASSERT(!fClipCoverageFP);
        fClipCoverageFP = fp;
    }

    void addStencilClip() {
        SkASSERT(!fHasStencilClip);
        fHasStencilClip = true;
    }

private:
    GrScissorState             fScissorState;
    GrWindowRectsState         fWindowRectsState;
    sk_sp<GrFragmentProcessor> fClipCoverageFP;
    bool                       fHasStencilClip = false;
    typedef SkNoncopyable INHERITED;
};

#endif
