package com.tds;
import android.widget.ImageView;
import android.view.ViewGroup;
import android.view.MotionEvent;
import android.view.View;
import android.content.Context;

import com.epicgames.ue4.GameActivity;
import com.tds.common.widgets.image.TdsImage;

class TapButtonPositionLayout extends ViewGroup
{
	public TapButtonPositionLayout(Context context, TapPlatformButton inButton)
	{
        super(context);
		button = inButton;
	}

	@Override
	protected void onLayout(boolean changed, int left, int top, int right, int bottom)
	{
		button.closeButton.layout(button.curX, button.curY, button.curX+button.curW, button.curY+button.curH);
	}

	private TapPlatformButton button;
}

public class TapPlatformButton
{
    public ImageView closeButton;
    private TapButtonPositionLayout positionLayout;
    public int curX, curY, curW, curH;
    public long nativePtr;
    public boolean PendingSetVisibility;
    public boolean InViewport;
    
    public TapPlatformButton(long inNativePtr)
    {
        nativePtr = inNativePtr;
        final TapPlatformButton w = this;
    
        PendingSetVisibility = false;
        InViewport = false;
    
        GameActivity.Get().runOnUiThread(new Runnable()
        {
            @Override
            public void run()
            {
                closeButton = new ImageView(GameActivity.Get());
                closeButton.setOnTouchListener(new View.OnTouchListener()
                {
                    @Override
                    public boolean onTouch(View v, MotionEvent event)
                    {
                        if(event.getAction() == MotionEvent.ACTION_DOWN)
                        {
                            NotifyButtonClicked(nativePtr);
                        }
                        return true;
                    }
                });
                
                positionLayout = new TapButtonPositionLayout(GameActivity.Get(), w);
                 
                curX = curY = curW = curH = 0;
            }
        });
    }
    
    public void Show()
    {
        if(!InViewport)
        {
            InViewport = true;
            GameActivity.Get().runOnUiThread(new Runnable()
            {
                @Override
                public void run()
                {
                    ViewGroup.LayoutParams params = new ViewGroup.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT, ViewGroup.LayoutParams.FILL_PARENT);
                    positionLayout.addView(closeButton, params);
                                 
                    ViewGroup.LayoutParams params2 = new ViewGroup.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT, ViewGroup.LayoutParams.FILL_PARENT);
                    GameActivity.Get().addContentView(positionLayout, params2);
                }
            });
        }
    }
    
    //Release
    public void Close()
    {
        if(InViewport)
        {
            InViewport = false;
            GameActivity.Get().runOnUiThread(new Runnable()
            {
            	@Override
            	public void run()
            	{
            	    ViewGroup parent = (ViewGroup)closeButton.getParent();
                    if (parent != null)
                    {
                    	parent.removeView(closeButton);
                    }
                    parent = (ViewGroup)positionLayout.getParent();
                    if (parent != null)
                    {
                    	parent.removeView(positionLayout);
                    }
            	}
            });
        }
    } 
    
    public void UpdateTexture(final String iconUrl)
    {
        GameActivity.Get().runOnUiThread(new Runnable()
        {
            @Override
            public void run()
            {
                TdsImage.get(GameActivity.Get())
                    .load(iconUrl)
                    .into(closeButton);
            }
        });
    }

    public void UpdateLayout(final int x, final int y, final int width, final int height)
    {
        GameActivity.Get().runOnUiThread(new Runnable()
        {
        	@Override
        	public void run()
        	{
        	    if( (closeButton != null) && (x != curX || y != curY || width != curW || height != curH))
                {
                	curX = x;
                	curY = y;
                	curW = width;
                	curH = height;
                	positionLayout.requestLayout();
                }
        	}
        });
    }
    
    public void SetVisible(boolean newVisible)
    {
        PendingSetVisibility = newVisible;
        GameActivity.Get().runOnUiThread(new Runnable()
        {
        	@Override
        	public void run()
        	{
        		closeButton.setVisibility(PendingSetVisibility? View.VISIBLE: View.GONE);
        	}
        });
    }
    
    public long GetNativePtr()
    {
        return nativePtr;
    }

    public native void NotifyButtonClicked(long nativePtr);
}

