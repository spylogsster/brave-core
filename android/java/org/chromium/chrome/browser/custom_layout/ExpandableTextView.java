/**
 * Copyright (c) 2023 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */
package org.chromium.chrome.browser.custom_layout;

import android.content.Context;
import android.text.SpannableStringBuilder;
import android.text.method.LinkMovementMethod;
import android.text.style.ClickableSpan;
import android.util.AttributeSet;
import android.view.View;
import android.view.ViewTreeObserver;

import androidx.appcompat.widget.AppCompatTextView;

import org.chromium.chrome.R;

public class ExpandableTextView extends AppCompatTextView {
    private final String ELLIP_TEXT = getContext().getString(R.string.ellip_text);
    private final String MORE_TEXT = getContext().getString(R.string.more_text);
    private final String LESS_TEXT = getContext().getString(R.string.less_text);

    private String mFullText;
    private int mMaxLines;

    public ExpandableTextView(Context context) {
        super(context);
    }

    public ExpandableTextView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public ExpandableTextView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }

    public void makeExpandable(String fullText, int maxLines) {
        mFullText = fullText;
        mMaxLines = maxLines;
        ViewTreeObserver vto = this.getViewTreeObserver();
        vto.addOnGlobalLayoutListener(new ViewTreeObserver.OnGlobalLayoutListener() {
            @Override
            public void onGlobalLayout() {
                ViewTreeObserver obs = getViewTreeObserver();
                obs.removeOnGlobalLayoutListener(this);
                if (getLineCount() <= maxLines) {
                    setText(mFullText);
                } else {
                    showLess();
                    setMovementMethod(LinkMovementMethod.getInstance());
                }
            }
        });
    }

    private void showLess() {
        int lineEndIndex = getLayout().getLineEnd(mMaxLines - 1);
        String newText = mFullText.substring(
                                 0, lineEndIndex - (ELLIP_TEXT.length() + MORE_TEXT.length() + 1))
                + ELLIP_TEXT + MORE_TEXT;
        SpannableStringBuilder builder = new SpannableStringBuilder(newText);
        builder.setSpan(new ClickableSpan() {
            @Override
            public void onClick(View widget) {
                showMore();
            }
        }, newText.length() - MORE_TEXT.length(), newText.length(), 0);
        setText(builder, BufferType.SPANNABLE);
    }

    private void showMore() {
        SpannableStringBuilder builder = new SpannableStringBuilder(mFullText + LESS_TEXT);
        builder.setSpan(new ClickableSpan() {
            @Override
            public void onClick(View widget) {
                showLess();
            }
        }, builder.length() - LESS_TEXT.length(), builder.length(), 0);
        setText(builder, BufferType.SPANNABLE);
    }
}
