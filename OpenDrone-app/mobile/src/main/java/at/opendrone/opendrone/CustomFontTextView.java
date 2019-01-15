package at.opendrone.opendrone;

import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Typeface;
import android.support.annotation.Nullable;
import android.support.v7.widget.AppCompatTextView;
import android.util.AttributeSet;

public class CustomFontTextView extends AppCompatTextView {
    public CustomFontTextView(Context context) {
        super(context);
        changeFont("Pacifico.ttf");
    }

    public CustomFontTextView(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        changeFont(attrs);
    }

    public CustomFontTextView(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        changeFont(attrs);
    }

    private void changeFont(String name){
        Typeface tf = Typeface.createFromAsset(getContext().getAssets(), "fonts/"+name);
        this.setTypeface(tf);
    }

    private void changeFont(AttributeSet attrs){
        TypedArray a=getContext().obtainStyledAttributes(attrs, R.styleable.CustomFontTextView);
        changeFont(a.getString(R.styleable.CustomFontTextView_customFont));
        a.recycle();
    }
}
