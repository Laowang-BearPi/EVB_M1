package com.mcz.greenhouse_chinese.app.model;

import android.content.Context;
import android.content.Intent;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.CompoundButton;
import android.widget.ImageView;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import com.daimajia.swipe.SwipeLayout;
import com.mcz.greenhouse_chinese.R;
import com.mcz.greenhouse_chinese.app.HistoricaldataActivity;
import com.mcz.greenhouse_chinese.app.utils.Config;
import com.mcz.greenhouse_chinese.app.utils.DataManager;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by mcz on 2018/1/10.
 */

public class PullrefreshListviewAdapter extends BaseAdapter {
    private Context mContext;
    private List<DataInfo> mlist=null;
    private String login_appid = "";
    private String token = "";
    private List<String> gatewayId=new ArrayList<String>();
    private List<String> deviceId=new ArrayList<String>();
    private String tuatus="201";
    private String deletetuatus="204";
    private String json=null;
    private ViewHolder vh;
    private int mposition;
    public  PullrefreshListviewAdapter( Context context){
        this.mContext=context;
        mlist=new ArrayList<DataInfo>();

    }

    public void settoken(String st){this.token=st;}
    public void setlogin_appid(String sl){this.login_appid=sl;}
    public void clearItem() {
        mlist.clear();
    }
    public void addItem(DataInfo item) {
        mlist.add(item);
    }
    @Override
    public int getCount() {
        return mlist.size();
    }

    @Override
    public DataInfo getItem(int position) {
        DataInfo item = null;
        if (position >= 0 && getCount() > position) {
            item = mlist.get(position);
        }
        return item;
    }

    @Override
    public long getItemId(int position) {
        return 0;
    }


    @Override
    public View getView(final int position, View convertView, ViewGroup parent) {


        if (convertView == null){
            convertView = LayoutInflater.from(mContext).inflate(R.layout.listview_item,parent,false);
            vh = new ViewHolder();
            vh.tv_name = (TextView) convertView.findViewById(R.id.txt_name_data);
            vh.tv_stutas = (TextView) convertView.findViewById(R.id.txt_status_data);
            vh.temperature = (TextView) convertView.findViewById(R.id.temperature);
            vh.tv_Devicehumidity = (TextView) convertView.findViewById(R.id.humidity);//数据id
            vh.tv_light = (TextView) convertView.findViewById(R.id.light);//数据id
            vh.swipeLayout=(SwipeLayout)convertView.findViewById(R.id.swipe);
            convertView.setTag(vh);
        }else{
            vh = (ViewHolder) convertView.getTag();
        }
        DataInfo info = getItem(position);
        vh.tv_name.setText(info.getDeviceName());
        if (info.getDeviceStatus().contains("OFFLINE")){
            vh.tv_stutas.setText("离线");
        }else {
            vh.tv_stutas.setText("在线");
        }

        deviceId.add(info.getDeviceId());
        gatewayId.add(info.getDeviceId());
        vh.swipeLayout.setClickToClose(true);
        vh.swipeLayout.close(true);
//        swipeLayout.open();
        vh.swipeLayout.setOnDoubleClickListener(new SwipeLayout.DoubleClickListener() {
            @Override
            public void onDoubleClick(SwipeLayout layout, boolean surface) {
                Intent mIntent = new Intent(mContext, HistoricaldataActivity.class);
//                String deviceId = mlist.get(position).getDeviceId();
//                String gatewayId = mlist.get(position).getGatewayId();
                mIntent.putExtra("deviceId", deviceId.get(position));
                mIntent.putExtra("gatewayId", gatewayId.get(position));
                mContext.startActivity(mIntent);

            }
        });

        vh.temperature.setText(info.getDevicetemperature());//设置数据temperature
        vh.tv_Devicehumidity.setText(info.getDevicehumidity());//设置数据humidity
        vh.tv_light.setText(info.getDeviceLight());//设置数据tv_light
        ImageView trash=(ImageView)convertView.findViewById(R.id.trash);
        final View finalConvertView = convertView;
        trash.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                new Thread(new Runnable() {
                    @Override
                    public void run() {

                        String add_url = Config.all_url + "/iocm/app/dm/v1.1.0/devices/"+deviceId.get(position)+"?appId=" + login_appid;
                        try {
                            json= DataManager.Delete_DEVICEID(mContext,add_url, login_appid, token);
                            if(json.equals(deletetuatus))
                            {


                                Looper.prepare();
                                mlist.remove(position);
                                Toast.makeText(finalConvertView.getContext(),"删除设备成功",Toast.LENGTH_SHORT).show();
                                deviceId.remove(position);
                                Message message=new Message();
                                message.what=1;
                                handler.sendMessage(message);
                                Looper.loop();

                            }
                            else
                            {
                                Looper.prepare();
                                Toast.makeText(finalConvertView.getContext(),"删除设备失败",Toast.LENGTH_SHORT).show();
                                Looper.loop();
                            }
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                }).start();
            }
        });

//////////////////////////////按键
        Switch switch_tmp_high=(Switch)convertView.findViewById(R.id.switch_tmp_high) ;
        switch_tmp_high.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(final CompoundButton compoundButton, boolean b) {
                if(b)
                {

                    new Thread(new Runnable() {
                        @Override
                        public void run() {
                            String add_url = Config.all_url + "/iocm/app/cmd/v1.4.0/deviceCommands?appId=" + login_appid;
                            //      String add_url = Config.all_url + "/iocm/app/cmd/v1.4.0/deviceCommands";
                            try {

                                json = DataManager.Comened_DEVICEID(mContext, add_url, login_appid, token,deviceId.get(position),"H_ON","Heat_Control","Heat");
                                Log.i("================","json   :      "+json);
                                if (json.equals(tuatus))
                                {
                                    Looper.prepare();
                                    Toast.makeText(compoundButton.getContext(),"下发成功",Toast.LENGTH_SHORT).show();
                                    Looper.loop();
                                }
                                else
                                {
                                    Looper.prepare();
                                    Toast.makeText(compoundButton.getContext(),"下发失败",Toast.LENGTH_SHORT).show();
                                    Looper.loop();
                                }


                            } catch (Exception e) {
                                e.printStackTrace();
                            }
                        }
                    }).start();
                }
                else
                {
                    new Thread(new Runnable() {
                        @Override
                        public void run() {
                            String add_url = Config.all_url + "/iocm/app/cmd/v1.4.0/deviceCommands?appId=" + login_appid;
                            try {

                                json = DataManager.Comened_DEVICEID(mContext, add_url, login_appid, token,deviceId.get(position),"H_OFF","Heat_Control","Heat");


                                if (json.equals(tuatus))
                                {
                                    Looper.prepare();
                                    Toast.makeText(compoundButton.getContext(),"下发成功",Toast.LENGTH_SHORT).show();
                                    Looper.loop();
                                }
                                else
                                {
                                    Looper.prepare();
                                    Toast.makeText(compoundButton.getContext(),"下发失败",Toast.LENGTH_SHORT).show();
                                    Looper.loop();
                                }

                            } catch (Exception e) {
                                e.printStackTrace();
                            }
                        }
                    }).start();


                }
            }
        });
        Switch switch_tmp_low=(Switch)convertView.findViewById(R.id.switch_tmp_low);
        switch_tmp_low.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(final CompoundButton compoundButton, boolean b) {
                if(b)
                {
                    new Thread(new Runnable() {
                        @Override
                        public void run() {
                            String add_url = Config.all_url + "/iocm/app/cmd/v1.4.0/deviceCommands?appId=" + login_appid;
                            //      String add_url = Config.all_url + "/iocm/app/cmd/v1.4.0/deviceCommands";
                            try {

                                json = DataManager.Comened_DEVICEID(mContext, add_url, login_appid, token,deviceId.get(position),"C_ON","Cool_Control","Cool");
                                Log.i("================","json   :      "+json);
                                if (json.equals(tuatus))
                                {
                                    Looper.prepare();
                                    Toast.makeText(compoundButton.getContext(),"下发成功",Toast.LENGTH_SHORT).show();
                                    Looper.loop();
                                }
                                else
                                {
                                    Looper.prepare();
                                    Toast.makeText(compoundButton.getContext(),"下发失败",Toast.LENGTH_SHORT).show();
                                    Looper.loop();
                                }


                            } catch (Exception e) {
                                e.printStackTrace();
                            }
                        }
                    }).start();
                }
                else
                {
                    new Thread(new Runnable() {
                        @Override
                        public void run() {
                            String add_url = Config.all_url + "/iocm/app/cmd/v1.4.0/deviceCommands?appId=" + login_appid;
                            try {

                                json = DataManager.Comened_DEVICEID(mContext, add_url, login_appid, token,deviceId.get(position),"C_OFF","Cool_Control","Cool");


                                if (json.equals(tuatus))
                                {
                                    Looper.prepare();
                                    Toast.makeText(compoundButton.getContext(),"下发成功",Toast.LENGTH_SHORT).show();
                                    Looper.loop();
                                }
                                else
                                {
                                    Looper.prepare();
                                    Toast.makeText(compoundButton.getContext(),"下发失败",Toast.LENGTH_SHORT).show();
                                    Looper.loop();
                                }

                            } catch (Exception e) {
                                e.printStackTrace();
                            }
                        }
                    }).start();

                }
            }
        });
        Switch switch_light=(Switch)convertView.findViewById(R.id.switch_light);
        switch_light.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(final CompoundButton compoundButton, boolean b) {

                if(b)
                {
                    new Thread(new Runnable() {
                        @Override
                        public void run() {
                            String add_url = Config.all_url + "/iocm/app/cmd/v1.4.0/deviceCommands?appId=" + login_appid;
                            //      String add_url = Config.all_url + "/iocm/app/cmd/v1.4.0/deviceCommands";
                            try {

                                json = DataManager.Comened_DEVICEID(mContext, add_url, login_appid, token,deviceId.get(position),"L_ON","Light_Control","Light");
                                Log.i("================","json   :      "+json);
                                if (json.equals(tuatus))
                                {
                                    Looper.prepare();
                                    Log.i("===================="," position       "+position);
                                    Toast.makeText(compoundButton.getContext(),"下发成功",Toast.LENGTH_SHORT).show();
                                    Looper.loop();
                                }
                                else
                                {
                                    Looper.prepare();
                                    Log.i("===================="," position       "+position);
                                    Toast.makeText(compoundButton.getContext(),"下发失败",Toast.LENGTH_SHORT).show();
                                    Looper.loop();
                                }


                            } catch (Exception e) {
                                e.printStackTrace();
                            }
                        }
                    }).start();
                }
                else
                {
                    new Thread(new Runnable() {
                        @Override
                        public void run() {
                            String add_url = Config.all_url + "/iocm/app/cmd/v1.4.0/deviceCommands?appId=" + login_appid;
                            try {

                                json = DataManager.Comened_DEVICEID(mContext, add_url, login_appid, token,deviceId.get(position),"L_OFF","Light_Control","Light");


                                if (json.equals(tuatus))
                                {
                                    Looper.prepare();
                                    Toast.makeText(compoundButton.getContext(),"下发成功",Toast.LENGTH_SHORT).show();
                                    Looper.loop();
                                }
                                else
                                {
                                    Looper.prepare();
                                    Toast.makeText(compoundButton.getContext(),"下发失败",Toast.LENGTH_SHORT).show();
                                    Looper.loop();
                                }

                            } catch (Exception e) {
                                e.printStackTrace();
                            }
                        }
                    }).start();

                }

            }
        });

//        aSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
//            @Override
//            public void onCheckedChanged(final CompoundButton buttonView, boolean isChecked) {
//               if (isChecked)
//               {
//                   new Thread(new Runnable() {
//                       @Override
//                       public void run() {
//
//                           String add_url = Config.all_url + "/iocm/app/cmd/v1.4.0/deviceCommands?appId=" + login_appid;
//                           try {
//
//                                json = DataManager.Comened_DEVICEID(mContext, add_url, login_appid, token,deviceId.get(position),"L_ON");
//                               if (json.equals(tuatus))
//                               {
//                                   Looper.prepare();
//                                   Toast.makeText(buttonView.getContext(),"下发成功",Toast.LENGTH_SHORT).show();
//                                   Looper.loop();
//                               }
//                               else
//                               {
//                                   Looper.prepare();
//                                   Toast.makeText(buttonView.getContext(),"下发失败",Toast.LENGTH_SHORT).show();
//                                   Looper.loop();
//                               }
//
//
//                           } catch (Exception e) {
//                               e.printStackTrace();
//                           }
//                       }
//                   }).start();
//
//
//               }
//               else
//               {
//                   new Thread(new Runnable() {
//                       @Override
//                       public void run() {
//
//                           String add_url = Config.all_url + "/iocm/app/cmd/v1.4.0/deviceCommands?appId=" + login_appid;
//                           try {
//
//                                json = DataManager.Comened_DEVICEID(mContext, add_url, login_appid, token,deviceId.get(position),"H_OFF");
//
//
//                               if (json.equals(tuatus))
//                               {
//                                   Looper.prepare();
//                                   Toast.makeText(buttonView.getContext(),"下发成功",Toast.LENGTH_SHORT).show();
//                                   Looper.loop();
//                               }
//                               else
//                               {
//                                   Looper.prepare();
//                                   Toast.makeText(buttonView.getContext(),"下发失败",Toast.LENGTH_SHORT).show();
//                                   Looper.loop();
//                               }
//
//                           } catch (Exception e) {
//                               e.printStackTrace();
//                           }
//                       }
//                   }).start();
//
//               }
//
//            }
//        });

        return convertView;
    }

    private Handler handler=new Handler()
    {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            switch (msg.what)
            {
                case 1:notifyDataSetChanged();
                    vh.swipeLayout.close(true);
                    break;
                default:break;
            }
        }

    };



    class ViewHolder{
        TextView tv_name;
        TextView tv_stutas;
        TextView tv_Devicehumidity,temperature ,tv_light;
        SwipeLayout swipeLayout;

    }
}
